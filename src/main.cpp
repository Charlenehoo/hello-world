#include "pch.h" // IWYU pragma: keep

namespace {
auto OnMessage(SKSE::MessagingInterface::Message *a_msg) -> void {
  if (a_msg == nullptr) {
    return;
  }

  switch (a_msg->type) {
  case SKSE::MessagingInterface::kDataLoaded: {
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
__declspec(dllexport) constinit auto SKSEPlugin_Version =
    []() noexcept -> SKSE::PluginVersionData {
  // NOLINTEND(readability-identifier-naming)
  SKSE::PluginVersionData data;
  data.PluginVersion(Plugin::kVersion);
  data.PluginName(Plugin::kName);
  data.AuthorName(Plugin::kAuthor);
  data.UsesAddressLibrary();
  data.UsesUpdatedStructs();
  data.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST});
  data.MinimumRequiredXSEVersion(REL::Version{2, 3, 0});
  return data;
}();