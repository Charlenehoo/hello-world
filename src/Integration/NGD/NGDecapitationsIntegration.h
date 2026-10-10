// src/Integration/NGD/NGDecapitationsIntegration.h

#pragma once

#include <NGDecapitationsAPI.h>

class NGDecapitationsIntegration {
public:
    ~NGDecapitationsIntegration() = default;
    NGDecapitationsIntegration(const NGDecapitationsIntegration&) = delete;
    NGDecapitationsIntegration(NGDecapitationsIntegration&&) = delete;
    auto operator=(const NGDecapitationsIntegration&) -> NGDecapitationsIntegration& = delete;
    auto operator=(NGDecapitationsIntegration&&) -> NGDecapitationsIntegration& = delete;

    static auto GetSingleton() -> NGDecapitationsIntegration&;

    // 在 kPostLoadGame / kNewGame 调用。
    void Initialize();

    [[nodiscard]] static auto IsAvailable() -> bool { return NGDecapitationsAPI::g_API != nullptr; }

    // ---- 参数类型别名：调用方不需要直接 #include 原始头 ----
    using DecapitateParams = NGDecapitationsAPI::DecapitateParams;

    // ---- 转发：调用方不接触 g_API 全局 ----
    static bool Decapitate(RE::Actor* a_target, DecapitateParams* a_params = nullptr) {
        return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->Decapitate(a_target, a_params);
    }

    [[nodiscard]] static bool IsDecapitated(RE::Actor* a_actor) {
        return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->IsDecapitated(a_actor);
    }

    [[nodiscard]] static bool IsHead(RE::Actor* a_actor) {
        return NGDecapitationsAPI::g_API != nullptr && NGDecapitationsAPI::g_API->IsHead(a_actor);
    }

    // ---- 逃生通道：NGD 以后加新方法，从这里拿指针顶上，不用等封装更新 ----
    [[nodiscard]] static auto GetAPI() -> NGDecapitationsAPI::NGDecapitationsAPI* { return NGDecapitationsAPI::g_API; }

private:
    NGDecapitationsIntegration() = default;

    bool m_initialized = false;
};