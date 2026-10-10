// src/Debug/CloneCorpse.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Debug/CloneCorpse.h"

#include <chrono>
#include <functional>
#include <future>
#include <string_view>
#include <thread>

namespace {
constexpr std::string_view kLogPrefix = "CloneAndOverlapStableCorpse: ";

// ---------- 基础工具 ----------

template <typename T>
auto ResolveHandleAs(const RE::ObjectRefHandle& a_handle) -> T* {
    auto ptr = a_handle ? a_handle.get() : nullptr;
    return ptr ? ptr->As<T>() : nullptr;
}

auto GetTraitTemplate(RE::TESNPC* a_base) -> RE::TESNPC* {
    auto* npc = a_base;
    while (npc && npc->faceNPC && npc->formID >= 0xFF000000) {
        npc = npc->faceNPC;
    }
    return npc;
}

auto GetRigidBody(RE::NiAVObject* a_node) -> RE::hkpRigidBody* {
    if (a_node == nullptr) {
        return nullptr;
    }
    auto* collisionObject = a_node->GetCollisionObject();
    if (collisionObject == nullptr) {
        return nullptr;
    }
    auto bhkBody = RE::NiPointer<RE::bhkRigidBody>(collisionObject->GetRigidBody());
    if (!bhkBody || !bhkBody->referencedObject) {
        return nullptr;
    }
    return static_cast<RE::hkpRigidBody*>(bhkBody->referencedObject.get());
}

auto IsReferenceRagdollReady(RE::TESObjectREFR* a_ref) -> bool {
    if (a_ref == nullptr || !a_ref->Is3DLoaded()) {
        return false;
    }
    auto* root = a_ref->Get3D();
    if (root == nullptr) {
        return false;
    }
    auto* body = GetRigidBody(root);
    return body != nullptr && body->world != nullptr && body->motion.GetMass() > 0.0f;
}

// ---------- 引擎直调 ----------

auto PlaceAtMe(RE::TESObjectREFR* a_self,
               RE::TESForm* a_form,
               std::uint32_t a_count = 1,
               bool a_forcePersist = false,
               bool a_initiallyDisabled = false) -> RE::TESObjectREFR* {
    using func_t = RE::TESObjectREFR* (*)(RE::BSScript::Internal::VirtualMachine*,
                                          RE::VMStackID,
                                          RE::TESObjectREFR*,
                                          RE::TESForm*,
                                          std::uint32_t,
                                          bool,
                                          bool);
    static REL::Relocation<func_t> func{RELOCATION_ID(55672, 56203)};
    auto* vm = RE::BSScript::Internal::VirtualMachine::GetSingleton();
    return func(vm, 0, a_self, a_form, a_count, a_forcePersist, a_initiallyDisabled);
}

auto GetFrameDelay() -> float {
    auto* timer = RE::BSTimer::GetSingleton();
    if (timer == nullptr) {
        return 0.00694444f;  // 144Hz
    }
    float delay = timer->realTimeDelta / timer->QGlobalTimeMultiplier();
    return std::clamp(delay, 0.004f, 0.1f);
}

// 阻塞当前线程直到主线程空闲（不在加载菜单里、不卡帧）。
void WaitForGameReady() {
    while (true) {
        if (auto* ui = RE::UI::GetSingleton(); ui && ui->GameIsPaused()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(GetFrameDelay() * 1000.0f)));
            continue;
        }

        std::promise<void> promise;
        auto future = promise.get_future();
        SKSE::GetTaskInterface()->AddTask([&promise]() { promise.set_value(); });

        auto start = std::chrono::high_resolution_clock::now();
        future.get();

        if ((std::chrono::high_resolution_clock::now() - start) > std::chrono::milliseconds(100)) {
            continue;  // 刚才在卡，重试
        }
        break;
    }
}

// ---------- 异步等待 ragdoll 就绪 ----------

template <typename TCallback>
void WaitUntilRagdollReady(RE::TESObjectREFR* a_ref,
                           TCallback&& a_callback,
                           std::chrono::milliseconds a_timeout = std::chrono::seconds(3)) {
    if (a_ref == nullptr) {
        a_callback(static_cast<RE::TESObjectREFR*>(nullptr), false);
        return;
    }

    std::jthread([formID = a_ref->formID, callback = std::forward<TCallback>(a_callback), a_timeout]() mutable {
        WaitForGameReady();

        const auto start = std::chrono::steady_clock::now();
        while (std::chrono::steady_clock::now() - start < a_timeout) {
            auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(formID);
            if (IsReferenceRagdollReady(ref)) {
                SKSE::GetTaskInterface()->AddTask([callback, ref]() { callback(ref, true); });
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(GetFrameDelay() * 1000.0f)));
        }

        SKSE::GetTaskInterface()->AddTask([callback, formID]() {
            auto* ref = RE::TESForm::LookupByID<RE::TESObjectREFR>(formID);
            callback(ref, false);
        });
    }).detach();
}

// ---------- 对齐 ----------

// 逐骨骼把 source 的 ragdoll 姿态复制到 target。
void CopyRagdollPose(RE::TESObjectREFR* a_target, RE::TESObjectREFR* a_source) {
    auto* sourceRoot = a_source->Get3D();
    if (sourceRoot == nullptr) {
        return;
    }

    std::size_t nodesCopied = 0;
    std::size_t motionsCopied = 0;

    RE::BSVisit::TraverseScenegraphObjects(sourceRoot, [&](RE::NiAVObject* a_node) -> RE::BSVisit::BSVisitControl {
        if (a_node == nullptr) {
            return RE::BSVisit::BSVisitControl::kContinue;
        }

        auto* targetNode = a_target->GetNodeByName(a_node->name);
        if (targetNode == nullptr) {
            return RE::BSVisit::BSVisitControl::kContinue;
        }

        // 1) 视觉对齐：直接复制 node 的变换
        targetNode->local = a_node->local;
        targetNode->world = a_node->world;
        ++nodesCopied;

        // 2) 物理对齐：写 motion state 的 transform 副本
        auto* sourceBody = GetRigidBody(a_node);
        auto* targetBody = GetRigidBody(targetNode);
        if (sourceBody != nullptr && targetBody != nullptr && sourceBody->world != nullptr &&
            targetBody->world != nullptr) {
            auto* sourceMS = sourceBody->GetMotionState();
            auto* targetMS = targetBody->GetMotionState();
            if (sourceMS != nullptr && targetMS != nullptr) {
                targetMS->transform = sourceMS->transform;
                ++motionsCopied;
            }
        }

        return RE::BSVisit::BSVisitControl::kContinue;
    });

    REX::INFO("{} copied {} nodes / {} motion states", kLogPrefix, nodesCopied, motionsCopied);
}
}  // namespace

namespace Debug {
void CloneAndOverlapStableCorpse(RE::Actor* a_source, std::function<void(RE::Actor*)> a_callback) {
    if (a_source == nullptr) {
        REX::WARN("{} source is null", kLogPrefix);
        if (a_callback) a_callback(nullptr);
        return;
    }
    if (!a_source->Is3DLoaded()) {
        REX::WARN("{} source {:08X} 3D not loaded", kLogPrefix, a_source->GetFormID());
        if (a_callback) a_callback(nullptr);
        return;
    }

    auto* base = GetTraitTemplate(a_source->GetActorBase());
    if (base == nullptr || base->formID >= 0xFF000000) {
        REX::WARN("{} source {:08X} has invalid base form", kLogPrefix, a_source->GetFormID());
        if (a_callback) a_callback(nullptr);
        return;
    }

    auto* refr = PlaceAtMe(a_source, base);
    auto* clone = refr ? refr->As<RE::Actor>() : nullptr;
    if (clone == nullptr) {
        REX::WARN("{} PlaceAtMe failed", kLogPrefix);
        if (a_callback) a_callback(nullptr);
        return;
    }

    clone->SetActivationBlocked(true);
    clone->SetDisplayName("", true);
    clone->SetScale(a_source->GetScale());
    clone->SetLifeState(RE::ACTOR_LIFE_STATE::kDead);

    REX::INFO("{} spawned clone {:08X} of source {:08X}", kLogPrefix, clone->GetFormID(), a_source->GetFormID());

    auto sourceHandle = a_source->GetHandle();

    WaitUntilRagdollReady(
        clone, [sourceHandle, callback = std::move(a_callback)](RE::TESObjectREFR* a_objectRef, const bool a_result) {
            if (!a_result || a_objectRef == nullptr) {
                REX::WARN("{} clone ragdoll never became ready", kLogPrefix);
                if (callback) callback(nullptr);
                return;
            }

            auto* clone = a_objectRef->As<RE::Actor>();
            auto* source = ResolveHandleAs<RE::Actor>(sourceHandle);
            if (clone == nullptr || source == nullptr) {
                REX::WARN("{} clone or source vanished during wait", kLogPrefix);
                if (callback) callback(nullptr);
                return;
            }

            CopyRagdollPose(clone, source);

            REX::INFO("{} clone {:08X} aligned to source {:08X}", kLogPrefix, clone->GetFormID(), source->GetFormID());

            if (callback) callback(clone);
        });
}
}  // namespace Debug