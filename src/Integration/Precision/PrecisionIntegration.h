// src/Integration/Precision/PrecisionIntegration.h

#pragma once

namespace PRECISION_API {
class IVPrecision4;
}

class PrecisionIntegration {
public:
    ~PrecisionIntegration() = default;
    PrecisionIntegration(const PrecisionIntegration&) = delete;
    PrecisionIntegration(PrecisionIntegration&&) = delete;
    auto operator=(const PrecisionIntegration&) -> PrecisionIntegration& = delete;
    auto operator=(PrecisionIntegration&&) -> PrecisionIntegration& = delete;

    static auto GetSingleton() -> PrecisionIntegration&;

    // 在 kDataLoaded 之后调用。请求 Precision API，注册命中回调。
    // Precision 未安装时只打 WARN，不影响插件其余部分。
    void Initialize();

private:
    PrecisionIntegration() = default;

    PRECISION_API::IVPrecision4* m_api = nullptr;
    SKSE::PluginHandle m_pluginHandle = 0;
};