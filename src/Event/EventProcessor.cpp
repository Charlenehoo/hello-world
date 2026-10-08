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
    for (auto* e = *a_event; e != nullptr; e = e->next) {
        // ...
    }
    return RE::BSEventNotifyControl::kContinue;
}