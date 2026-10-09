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

auto EventProcessor::ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* /*a_source*/)
    -> RE::BSEventNotifyControl {
    if (a_event == nullptr) {
        return RE::BSEventNotifyControl::kContinue;
    }

    const auto actorFormID = a_event->actor != nullptr ? a_event->actor->GetFormID() : 0;
    const char* actorName = a_event->actor != nullptr ? a_event->actor->GetName() : nullptr;

    const auto* baseForm = a_event->baseObject != 0 ? RE::TESForm::LookupByID(a_event->baseObject) : nullptr;
    const char* baseName = baseForm != nullptr ? baseForm->GetName() : nullptr;

    REX::DEBUG("TESEquipEvent: actor={:08X} '{}' base={:08X} '{}' originalRefr={:08X} uniqueID={} equipped={}",
               actorFormID,
               actorName != nullptr ? actorName : "?",
               a_event->baseObject,
               baseName != nullptr ? baseName : "?",
               a_event->originalRefr,
               a_event->uniqueID,
               a_event->equipped);

    return RE::BSEventNotifyControl::kContinue;
}

// ============================================================
//  TESObjectLoadedEvent —— 物件被加载进/移出内存
// ============================================================

auto EventProcessor::ProcessEvent(const RE::TESObjectLoadedEvent* a_event,
                                  RE::BSTEventSource<RE::TESObjectLoadedEvent>* /*a_source*/)
    -> RE::BSEventNotifyControl {
    if (a_event == nullptr) {
        return RE::BSEventNotifyControl::kContinue;
    }

    const auto* form = a_event->formID != 0 ? RE::TESForm::LookupByID(a_event->formID) : nullptr;
    const char* name = form != nullptr ? form->GetName() : nullptr;

    REX::DEBUG("TESObjectLoadedEvent: formID={:08X} '{}' loaded={}",
               a_event->formID,
               name != nullptr ? name : "?",
               a_event->loaded);

    return RE::BSEventNotifyControl::kContinue;
}