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

    using DecapitateParams = NGDecapitationsAPI::DecapitateParams;

    [[nodiscard]] static auto IsAvailable() -> bool;
    [[nodiscard]] static auto GetAPI() -> NGDecapitationsAPI::NGDecapitationsAPI*;
    static auto Decapitate(RE::Actor* a_target, DecapitateParams* a_params = nullptr) -> bool;
    [[nodiscard]] static auto IsDecapitated(RE::Actor* a_actor) -> bool;
    [[nodiscard]] static auto IsHead(RE::Actor* a_actor) -> bool;

private:
    NGDecapitationsIntegration() = default;

    bool m_initialized = false;
};