// src/Debug/DecapitateTest.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Debug/DecapitateTest.h"

#include "Integration/NGD/NGDecapitationsIntegration.h"
#include "RE/C/CrosshairPickData.h"


namespace Debug {
void DecapitateCrosshairTarget() {
    auto* player = RE::PlayerCharacter::GetSingleton();
    if (player == nullptr) {
        REX::WARN("DecapitateTest: player is null");
        return;
    }

    // 准星目标从 CrosshairPickData 里取。
    // 如果这个类型名或成员名与你的 CommonLibSSE-NG 版本不符，
    // 编译报错时去 extern/CommonLibSSE/include/RE/C/ 下核对。
    auto* pickData = RE::CrosshairPickData::GetSingleton();
    if (pickData == nullptr) {
        REX::WARN("DecapitateTest: CrosshairPickData is null");
        return;
    }

    auto targetHandle = pickData->target;
    auto* target = targetHandle.get().get();
    if (target == nullptr) {
        REX::INFO("DecapitateTest: nothing under crosshair");
        return;
    }

    auto* actor = target->As<RE::Actor>();
    if (actor == nullptr) {
        REX::INFO("DecapitateTest: crosshair target {:08X} is not an Actor", target->GetFormID());
        return;
    }

    REX::INFO("DecapitateTest: target actor={:08X} '{}' dead={}",
              actor->GetFormID(),
              actor->GetName() != nullptr ? actor->GetName() : "?",
              actor->IsDead());

    if (!NGDecapitationsIntegration::IsAvailable()) {
        REX::WARN("DecapitateTest: NGD not available; aborting");
        return;
    }

    NGDecapitationsIntegration::DecapitateParams params;
    // customHitData = false（默认）→ 走 NGD 默认的冲量逻辑。
    // 想试自定义冲量就设 customHitData = true 并填 hitFrom/hitTo/hitPower。
    params.callback = [](RE::Actor* a_head) {
        if (a_head != nullptr) {
            REX::INFO("DecapitateTest: callback fired, head={:08X}", a_head->GetFormID());
        }
    };

    if (NGDecapitationsIntegration::Decapitate(actor, &params)) {
        REX::INFO("DecapitateTest: NGD accepted the call");
    } else {
        REX::WARN("DecapitateTest: NGD rejected the call (already decapitated, race not patched, ...)");
    }
}
}  // namespace Debug