// src/Integration/Precision/PrecisionIntegration.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Integration/Precision/PrecisionIntegration.h"

#include <PrecisionAPI.h>

namespace {
void LogHitData(const PRECISION_API::PrecisionHitData& a_data) {
    const auto attackerFormID = a_data.attacker != nullptr ? a_data.attacker->GetFormID() : 0;
    const char* attackerName = a_data.attacker != nullptr ? a_data.attacker->GetName() : nullptr;

    const auto targetFormID = a_data.target != nullptr ? a_data.target->GetFormID() : 0;
    const char* targetName = a_data.target != nullptr ? a_data.target->GetName() : nullptr;

    REX::DEBUG(
        "Precision hit: attacker={:08X} '{}' target={:08X} '{}' "
        "hitPos=({:.2f}, {:.2f}, {:.2f}) "
        "separatingNormal=({:.2f}, {:.2f}, {:.2f}) "
        "hitPointVelocity=({:.2f}, {:.2f}, {:.2f}) "
        "hitShapeKey={} hittingShapeKey={}",
        attackerFormID,
        attackerName != nullptr ? attackerName : "?",
        targetFormID,
        targetName != nullptr ? targetName : "?",
        a_data.hitPos.x,
        a_data.hitPos.y,
        a_data.hitPos.z,
        a_data.separatingNormal.x,
        a_data.separatingNormal.y,
        a_data.separatingNormal.z,
        a_data.hitPointVelocity.x,
        a_data.hitPointVelocity.y,
        a_data.hitPointVelocity.z,
        static_cast<std::uint32_t>(a_data.hitBodyShapeKey),
        static_cast<std::uint32_t>(a_data.hittingBodyShapeKey));
}
}  // namespace

auto PrecisionIntegration::GetSingleton() -> PrecisionIntegration& {
    static PrecisionIntegration s_singleton;
    return s_singleton;
}

void PrecisionIntegration::Initialize() {
    m_pluginHandle = SKSE::GetPluginHandle();

    auto* api =
        static_cast<PRECISION_API::IVPrecision4*>(PRECISION_API::RequestPluginAPI(PRECISION_API::InterfaceVersion::V4));
    if (api == nullptr) {
        REX::WARN("PrecisionIntegration: Precision not available, integration disabled");
        return;
    }

    m_api = api;

    const auto postHitResult = api->AddPostHitCallback(
        m_pluginHandle,
        [](const PRECISION_API::PrecisionHitData& a_data, const RE::HitData& /*a_hitData*/) { LogHitData(a_data); });

    if (postHitResult != PRECISION_API::APIResult::OK) {
        REX::WARN("PrecisionIntegration: AddPostHitCallback failed ({})", static_cast<int>(postHitResult));
    }

    REX::INFO("PrecisionIntegration: initialized (pluginHandle={})", m_pluginHandle);
}