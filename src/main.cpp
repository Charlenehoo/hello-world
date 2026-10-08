#include "pch.h"  // IWYU pragma: keep

#include "Event/EventProcessor.h"

namespace {
void RegisterEvents() {
    auto* holder = RE::ScriptEventSourceHolder::GetSingleton();
    if (holder == nullptr) {
        REX::WARN("RegisterEvents: ScriptEventSourceHolder is nullptr");
        return;
    }

    auto& processor = EventProcessor::GetSingleton();
    holder->AddEventSink<RE::TESEquipEvent>(&processor);
    holder->AddEventSink<RE::TESObjectLoadedEvent>(&processor);

    REX::INFO("RegisterEvents: Event sinks registered");
}

void OnMessage(SKSE::MessagingInterface::Message* a_msg) {
    if (a_msg == nullptr) {
        REX::WARN("OnMessage: Message is nullptr");
        return;
    }

    switch (a_msg->type) {
        case SKSE::MessagingInterface::kDataLoaded: {
            REX::TRACE("OnMessage: DataLoaded");

            RegisterEvents();
            break;
        }
        default:
            break;
    }
}
}  // namespace

// NOLINTBEGIN(readability-identifier-naming)
extern "C" [[maybe_unused]] __declspec(dllexport) auto SKSEPlugin_Load(const SKSE::LoadInterface* a_skse) -> bool {
    // NOLINTEND(readability-identifier-naming)
    if (a_skse == nullptr) {
        return false;
    }

    SKSE::Init(a_skse,
               SKSE::InitInfo{
                   .log = true,
               });

    const auto* messaging = SKSE::GetMessagingInterface();
    if (messaging == nullptr) {
        REX::CRITICAL("SKSEPlugin_Load: GetMessagingInterface returned nullptr");
        return false;
    }

    if (!messaging->RegisterListener(OnMessage)) {
        REX::CRITICAL("SKSEPlugin_Load: RegisterListener failed");
        return false;
    }

    REX::INFO("SKSEPlugin_Load: loaded");
    return true;
}

// NOLINTBEGIN(readability-identifier-naming)
extern "C" [[maybe_unused]] __declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version = []() noexcept {
    // NOLINTEND(readability-identifier-naming)
    SKSE::PluginVersionData data;
    data.PluginVersion(Plugin::kVersion);
    data.PluginName(Plugin::kName);
    data.AuthorName(Plugin::kAuthor);
    data.AuthorEmail(Plugin::kAuthorEmail);
    data.UsesAddressLibrary();
    data.UsesUpdatedStructs();
    data.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST});
    data.MinimumRequiredXSEVersion(REL::Version{2, 3, 0});
    return data;
}();