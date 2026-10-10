// src/Integration/NGD/NGDecapitationsIntegration.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Integration/NGD/NGDecapitationsIntegration.h"

auto NGDecapitationsIntegration::GetSingleton() -> NGDecapitationsIntegration& {
    static NGDecapitationsIntegration s_singleton;
    return s_singleton;
}

void NGDecapitationsIntegration::Initialize() {
    if (m_initialized) {
        return;
    }
    m_initialized = true;

    if (!NGDecapitationsAPI::LoadAPI()) {
        REX::INFO("NGDecapitationsIntegration: NGD not installed or version mismatch; disabled");
        return;
    }

    REX::INFO("NGDecapitationsIntegration: initialized (apiVersion={:08X})", NGDecapitationsAPI::g_API->GetVersion());
}

auto NGDecapitationsIntegration::IsAvailable() -> bool { return NGDecapitationsAPI::g_API != nullptr; }

auto NGDecapitationsIntegration::GetAPI() -> NGDecapitationsAPI::NGDecapitationsAPI* {
    return NGDecapitationsAPI::g_API;
}

auto NGDecapitationsIntegration::Decapitate(RE::Actor* a_target, DecapitateParams* a_params) -> bool {
    return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->Decapitate(a_target, a_params);
}

auto NGDecapitationsIntegration::IsDecapitated(RE::Actor* a_actor) -> bool {
    return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->IsDecapitated(a_actor);
}

auto NGDecapitationsIntegration::IsHead(RE::Actor* a_actor) -> bool {
    return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->IsHead(a_actor);
}