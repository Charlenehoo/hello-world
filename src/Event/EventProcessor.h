#pragma once

#include <RE/B/BSTEvent.h>
#include <RE/I/InputEvent.h>
#include <RE/T/TESEquipEvent.h>
#include <RE/T/TESObjectLoadedEvent.h>

class EventProcessor : public RE::BSTEventSink<RE::InputEvent*>,
                       public RE::BSTEventSink<RE::TESEquipEvent>,
                       public RE::BSTEventSink<RE::TESObjectLoadedEvent> {
public:
    ~EventProcessor() = default;
    EventProcessor(const EventProcessor&) = delete;
    EventProcessor(EventProcessor&&) = delete;
    auto operator=(const EventProcessor&) -> EventProcessor& = delete;
    auto operator=(EventProcessor&&) -> EventProcessor& = delete;

    static auto GetSingleton() -> EventProcessor&;

    auto ProcessEvent(const RE::TESEquipEvent* a_event, RE::BSTEventSource<RE::TESEquipEvent>* a_source)
        -> RE::BSEventNotifyControl override;
    auto ProcessEvent(const RE::TESObjectLoadedEvent* a_event, RE::BSTEventSource<RE::TESObjectLoadedEvent>* a_source)
        -> RE::BSEventNotifyControl override;
    auto ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_source)
        -> RE::BSEventNotifyControl override;

private:
    EventProcessor() = default;
};