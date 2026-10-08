// src/Event/EventProcessor.cpp

#include "pch.h"  // IWYU pragma: keep

#include "Event/EventProcessor.h"

auto EventProcessor::GetSingleton() -> EventProcessor& {
    static EventProcessor s_singleton;
    return s_singleton;
}

// ============================================================
//  TESEquipEvent —— 玩家/NPC 装备或卸下物品
// ============================================================

auto EventProcessor::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_source)
    -> RE::BSEventNotifyControl {
    return RE::BSEventNotifyControl::kContinue;
}

// ============================================================
//  TESObjectLoadedEvent —— 物件被加载进/移出内存
// ============================================================

auto EventProcessor::ProcessEvent(const RE::TESObjectLoadedEvent* a_event,
                                  RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_source) -> RE::BSEventNotifyControl {
    return RE::BSEventNotifyControl::kContinue;
}

auto EventProcessor::ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
    -> RE::BSEventNotifyControl {
    if (a_event == nullptr) {
        return RE::BSEventNotifyControl::kContinue;
    }

    auto* inputMgr = RE::BSInputDeviceManager::GetSingleton();

    for (auto* event = *a_event; event != nullptr; event = event->next) {
        if (event->GetEventType() != RE::INPUT_EVENT_TYPE::kButton) {
            continue;
        }

        auto* button = event->AsButtonEvent();
        if (button == nullptr || !button->HasIDCode()) {
            continue;
        }

        const auto device = button->GetDevice();
        const auto key = button->GetIDCode();

        // 从游戏的按键映射里取人类可读名
        RE::BSFixedString name;
        if (inputMgr == nullptr || !inputMgr->GetButtonNameFromID(device, static_cast<std::int32_t>(key), name)) {
            name = "?";  // 直接赋 const char*，BSFixedString 有 operator=
        }

        REX::DEBUG("device={} key={:>4} name={:<12} {}  value={:.2f}  held={:.3f}s",
                   static_cast<int>(device),
                   key,
                   name.c_str(),
                   button->IsDown() ? "DOWN" : "UP  ",
                   button->value,
                   button->heldDownSecs);
    }

    return RE::BSEventNotifyControl::kContinue;
}