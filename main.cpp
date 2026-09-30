#include "pch.h" // IWYU pragma: keep

extern "C" [[maybe_unused]] __declspec(dllexport) auto
SKSEPlugin_Load(const SKSE::LoadInterface *a_skse) -> bool {
  SKSE::Init(a_skse, SKSE::InitInfo{
                         .log = true,
                     });
  const auto *messaging = SKSE::GetMessagingInterface();
  if (!messaging) {
    return false;
  }

  if (!messaging->RegisterListener(
          [](SKSE::MessagingInterface::Message *a_msg) -> void {
            if (!a_msg) {
              return;
            }

            switch (a_msg->type) {
            case SKSE::MessagingInterface::kDataLoaded: {
              auto *console = RE::ConsoleLog::GetSingleton();
              if (console) {
                console->Print("Hello world");
              }
              break;
            }
            default:
              break;
            }
          })) {
    return false;
  }

  return true;
}

extern "C" [[maybe_unused]]
__declspec(dllexport) constinit auto SKSEPlugin_Version =
    []() -> SKSE::PluginVersionData {
  SKSE::PluginVersionData data;
  data.PluginVersion(REL::Version{2, 3, 0});
  data.PluginName("Hello");
  data.AuthorName("Charlene Hoo");
  data.UsesAddressLibrary();
  data.UsesUpdatedStructs();
  data.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST});
  data.MinimumRequiredXSEVersion(REL::Version{2, 3, 0});
  return data;
}();