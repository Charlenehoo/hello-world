// src/Integration/SKEE/SKEEIntegration.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Integration/SKEE/SKEEIntegration.h"

#include <IPluginInterface.h>
#include <cstdint>

namespace {
// SKEE 内部注册名，非 C++ 类名。换 RaceMenu 版本若接口取不到，
// 优先核对这里的字符串。
constexpr const char* kSKEEPluginName = "SKEE";
constexpr const char* kBodyMorphInterfaceName = "BodyMorph";

// 我们目前用到的方法的最低版本要求。上游 BodyMorph 接口当前最新为
// kPluginVersion5，取 4 留余量，后续按需调整。
constexpr SKEE::skee_u32 kBodyMorphMinVersion = 4;
}  // namespace

auto SKEEIntegration::GetSingleton() -> SKEEIntegration& {
    static SKEEIntegration s_singleton;
    return s_singleton;
}

void SKEEIntegration::Initialize() {
    const auto* messaging = SKSE::GetMessagingInterface();
    if (messaging == nullptr) {
        REX::WARN("SKEEIntegration: GetMessagingInterface returned nullptr");
        return;
    }

    SKEE::InterfaceExchangeMessage exchange{};
    messaging->Dispatch(static_cast<std::uint32_t>(SKEE::InterfaceExchangeMessage::kMessage_ExchangeInterface),
                        &exchange,
                        sizeof(exchange),
                        kSKEEPluginName);

    if (exchange.interfaceMap == nullptr) {
        REX::WARN("SKEEIntegration: SKEE not available, integration disabled");
        return;
    }

    // NOLINTBEGIN(cppcoreguidelines-pro-type-static-cast-downcast)
    auto* bodyMorph =
        static_cast<SKEE::IBodyMorphInterface*>(exchange.interfaceMap->QueryInterface(kBodyMorphInterfaceName));
    // NOLINTEND(cppcoreguidelines-pro-type-static-cast-downcast)
    // SKEE 接口查询协议，name→type 契约确定，跨 DLL 无 RTTI
    if (bodyMorph == nullptr) {
        REX::WARN("SKEEIntegration: BodyMorph interface not found");
        return;
    }

    const auto version = bodyMorph->GetVersion();
    if (version < kBodyMorphMinVersion) {
        REX::WARN("SKEEIntegration: BodyMorph version too old (need >= {}, got {})", kBodyMorphMinVersion, version);
        return;
    }

    m_bodyMorph = bodyMorph;
    REX::INFO("SKEEIntegration: initialized (bodyMorphVersion={})", version);
}