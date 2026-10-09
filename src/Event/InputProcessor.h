// src/Event/InputProcessor.h

#pragma once

#include <RE/B/BSTEvent.h>
#include <RE/I/InputEvent.h>
#include <array>
#include <atomic>
#include <cstddef>

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

    // 输入事件跨线程分发，用固定大小的原子数组替代 unordered_map：
    // 数组无 rehash、无节点分配、无迭代器失效，每个槽位独立读改写。
    // 索引：[device][code]，越界的直接忽略。
    static constexpr std::size_t kMaxDevices = 4;  // kKeyboard / kMouse / kGamepad / kVirtualKeyboard
    static constexpr std::size_t kMaxCode = 512;   // 键盘最大 ~0x40，鼠标 <0x10，手柄 <0x200

    std::array<std::array<std::atomic<bool>, kMaxCode>, kMaxDevices> m_keyState{};
};