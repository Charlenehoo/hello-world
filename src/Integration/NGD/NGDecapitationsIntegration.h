// src/Integration/NGD/NGDecapitationsIntegration.h

#pragma once

#include <NGDecapitationsAPI.h>

class NGDecapitationsIntegration {
public:
    // 纯静态类，不需要实例。
    NGDecapitationsIntegration() = delete;

    using DecapitateParams = NGDecapitationsAPI::DecapitateParams;

    // 懒加载：首次调用任意 API 时探测 NGD，之后返回缓存结果。
    [[nodiscard]] static auto IsAvailable() -> bool;
    [[nodiscard]] static auto GetAPI() -> NGDecapitationsAPI::NGDecapitationsAPI*;
    static auto Decapitate(RE::Actor* a_target, DecapitateParams* a_params = nullptr) -> bool;
    [[nodiscard]] static auto IsDecapitated(RE::Actor* a_actor) -> bool;
    [[nodiscard]] static auto IsHead(RE::Actor* a_actor) -> bool;
};
