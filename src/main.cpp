#include "pch.h" // IWYU pragma: keep

#include "Event/EventProcessor.h"

namespace {
void RegisterEvents() {
  auto *holder = RE::ScriptEventSourceHolder::GetSingleton();
  if (holder == nullptr) {
    REX::WARN("[Event] ScriptEventSourceHolder is null, skipped.");
    return;
  }

  auto &processor = EventProcessor::GetSingleton();
  holder->AddEventSink<RE::TESEquipEvent>(std::addressof(processor));
  holder->AddEventSink<RE::TESObjectLoadedEvent>(std::addressof(processor));

  REX::INFO("[Event] EventProcessor registered.");
}

void OnMessage(SKSE::MessagingInterface::Message *a_msg) {
  if (a_msg == nullptr) {
    return;
  }

  switch (a_msg->type) {
  case SKSE::MessagingInterface::kDataLoaded: {
    RegisterEvents();

    auto *console = RE::ConsoleLog::GetSingleton();
    if (console != nullptr) {
      console->Print("Hello world");
    }
    break;
  }
  default:
    break;
  }
}
} // namespace

// NOLINTBEGIN(readability-identifier-naming)
extern "C" [[maybe_unused]] __declspec(dllexport) auto
SKSEPlugin_Load(const SKSE::LoadInterface *a_skse) -> bool {
  // NOLINTEND(readability-identifier-naming)
  if (a_skse == nullptr) {
    return false;
  }

  SKSE::Init(a_skse, SKSE::InitInfo{
                         .log = true,
                     });
  const auto *messaging = SKSE::GetMessagingInterface();
  if (messaging == nullptr) {
    return false;
  }

  if (!messaging->RegisterListener(OnMessage)) {
    return false;
  }

  return true;
}

// NOLINTBEGIN(readability-identifier-naming)
extern "C" [[maybe_unused]]
__declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version =
    []() noexcept {
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