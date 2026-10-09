// src/Event/InputProcessor.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Event/InputProcessor.h"

auto InputProcessor::GetSingleton() -> InputProcessor& {
    static InputProcessor s_singleton;
    return s_singleton;
}

auto InputProcessor::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* /*a_source*/)
    -> RE::BSEventNotifyControl {
    if (a_event == nullptr) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto* inputMgr = RE::BSInputDeviceManager::GetSingleton();

    // value 是模拟量（0.0 ~ 1.0），键盘一般为 0 或 1；
    // 用 0.5 阈值过滤手柄摇杆抖动。
    constexpr float kPressedThreshold = 0.5F;

    for (auto* event = *a_event; event != nullptr; event = event->next) {
        if (event->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
            continue;
        }

        auto* button = event->AsButtonEvent();
        if (button == nullptr || !button->HasIDCode()) {
            continue;
        }

        const auto device = button->GetDevice();
        const auto code = button->GetIDCode();

        const auto dev = static_cast<std::size_t>(device);
        const auto idx = static_cast<std::size_t>(code);
        if (dev >= kMaxDevices || idx >= kMaxCode) {
            REX::WARN("InputProcessor: out-of-range device={} code={} (kMaxDevices={}, kMaxCode={}); enlarge the array",
                      static_cast<int>(device),
                      code,
                      kMaxDevices,
                      kMaxCode);
            continue;
        }

        const bool isDown = button->value > kPressedThreshold;

        // .at() 而非 []：满足 tidy 的边界安全要求。
        // 上面已经做过边界检查，这里的 .at() 不可能抛。
        auto& slot = m_keyState.at(dev).at(idx);

        // CAS：只有"当前状态 != isDown"时才成功，等价于"发生了跳变"。
        // 成功后槽位被写入 isDown；失败说明状态没变（重复事件），跳过。
        bool expected = !isDown;
        if (!slot.compare_exchange_strong(expected, isDown, std::memory_order_relaxed)) {
            continue;
        }

        // 只有状态真的变了才解析名字、打日志
        RE::BSFixedString name;
        if (inputMgr == nullptr || !inputMgr->GetButtonNameFromID(device, static_cast<std::int32_t>(code), name)) {
            name = "?";
        }

        REX::DEBUG("device={} key={:>4} name={:<12} {}  value={:.2f}  held={:.3f}s",
                   static_cast<int>(device),
                   code,
                   name.c_str(),
                   isDown ? "DOWN" : "UP  ",
                   button->value,
                   button->heldDownSecs);
    }

    return RE::BSEventNotifyControl::kContinue;
}