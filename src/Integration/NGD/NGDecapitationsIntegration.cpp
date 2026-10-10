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