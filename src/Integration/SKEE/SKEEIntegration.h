// src/Integration/SKEE/SKEEIntegration.h

#pragma once

class SKEEIntegration {
public:
    ~SKEEIntegration() = default;
    SKEEIntegration(const SKEEIntegration&) = delete;
    SKEEIntegration(SKEEIntegration&&) = delete;
    auto operator=(const SKEEIntegration&) -> SKEEIntegration& = delete;
    auto operator=(SKEEIntegration&&) -> SKEEIntegration& = delete;

    static auto GetSingleton() -> SKEEIntegration&;

    // 在 kPostPostLoad 调用。主动向 SKEE 发起接口交换请求，
    // 只有 SKEE 的 listener 会响应。SKEE 未安装时只打 WARN，
    // 不影响插件其他功能。
    void Initialize();

private:
    SKEEIntegration() = default;

    void* m_bodyMorph = nullptr;
};