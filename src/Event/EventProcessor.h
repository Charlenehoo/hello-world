// src/Event/EventProcessor.h
#pragma once

class EventProcessor : public RE::BSTEventSink<RE::TESEquipEvent>, public RE::BSTEventSink<RE::TESObjectLoadedEvent> {
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

private:
    EventProcessor() = default;
};