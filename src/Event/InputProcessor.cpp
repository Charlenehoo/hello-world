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

        const bool isDown = button->value > kPressedThreshold;

        const KeyId keyId{.m_device = device, .m_code = code};
        const auto iter = m_keyState.find(keyId);
        const bool wasDown = (iter != m_keyState.end()) ? iter->second : false;

        // 电平没变 → 按住/空闲期间的重复事件，跳过
        if (isDown == wasDown) {
            continue;
        }

        m_keyState[keyId] = isDown;

        // 只有跳变时才解析名字、打日志
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