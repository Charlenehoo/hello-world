// src/Integration/NGD/NGDecapitationsIntegration.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Integration/NGD/NGDecapitationsIntegration.h"

namespace {
// 唯一真状态：NGD API 是否已探测且可用。
// static 局部变量保证 lambda 在整个进程生命周期内只执行一次（C++11 起线程安全）。
// 失败也缓存，不会因为后续调用而重试 Dispatch。
auto EnsureLoaded() -> bool {
    static bool s_apiAvailable = [] {
        if (!NGDecapitationsAPI::LoadAPI()) {
            REX::INFO("NGDecapitationsIntegration: NGD not installed or version mismatch; disabled");
            return false;
        }
        REX::INFO("NGDecapitationsIntegration: initialized (apiVersion={:08X})",
                  NGDecapitationsAPI::g_API->GetVersion());
        return true;
    }();
    return s_apiAvailable;
}
}  // namespace

auto NGDecapitationsIntegration::IsAvailable() -> bool { return EnsureLoaded(); }

auto NGDecapitationsIntegration::GetAPI() -> NGDecapitationsAPI::NGDecapitationsAPI* {
    return EnsureLoaded() ? NGDecapitationsAPI::g_API : nullptr;
}

auto NGDecapitationsIntegration::Decapitate(RE::Actor* a_target, DecapitateParams* a_params) -> bool {
    if (!EnsureLoaded()) {
        return false;
    }
    return NGDecapitationsAPI::g_API->Decapitate(a_target, a_params);
}

auto NGDecapitationsIntegration::IsDecapitated(RE::Actor* a_actor) -> bool {
    if (!EnsureLoaded()) {
        return false;
    }
    return NGDecapitationsAPI::g_API->IsDecapitated(a_actor);
}

auto NGDecapitationsIntegration::IsHead(RE::Actor* a_actor) -> bool {
    if (!EnsureLoaded()) {
        return false;
    }
    return NGDecapitationsAPI::g_API->IsHead(a_actor);
}
