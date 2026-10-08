// src/Event/InputProcessor.h

#pragma once

#include <RE/B/BSTEvent.h>
#include <RE/I/InputEvent.h>
#include <cstdint>
#include <unordered_map>


class InputProcessor : public RE::BSTEventSink<RE::InputEvent*> {
public:
    ~InputProcessor() = default;
    InputProcessor(const InputProcessor&) = delete;
    InputProcessor(InputProcessor&&) = delete;
    auto operator=(const InputProcessor&) -> InputProcessor& = delete;
    auto operator=(InputProcessor&&) -> InputProcessor& = delete;

    static auto GetSingleton() -> InputProcessor&;

    auto ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
        -> RE::BSEventNotifyControl override;

private:
    InputProcessor() = default;

    // device 和 code 都是小整数，直接打包成 32 位即可唯一标识一个物理按键。
    // 高 8 位放 device，低 24 位放 code。
    struct KeyId {
        RE::INPUT_DEVICE m_device;
        std::uint32_t m_code;

        auto operator==(const KeyId& a_other) const noexcept -> bool {
            return m_device == a_other.m_device && m_code == a_other.m_code;
        }

        [[nodiscard]] auto Hash() const noexcept -> std::size_t {
            constexpr int kDeviceShift = 24;
            return (static_cast<std::uint32_t>(m_device) << kDeviceShift) | m_code;
        }
    };

    struct KeyIdHash {
        auto operator()(const KeyId& a_key) const noexcept -> std::size_t { return a_key.Hash(); }
    };

    // 每个键“上一次是否按下”。只在 false<->true 跳变时输出日志。
    std::unordered_map<KeyId, bool, KeyIdHash> m_keyState;
};