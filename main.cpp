#include "pch.h" // IWYU pragma: keep

extern "C" [[maybe_unused]] __declspec(dllexport) auto
SKSEPlugin_Load(const SKSE::LoadInterface *a_skse) -> bool {
  return true;
}

extern "C" [[maybe_unused]]
__declspec(dllexport) constinit SKSE::PluginVersionData SKSEPlugin_Version =
    []() {
      SKSE::PluginVersionData data;
      // v.PluginVersion();
      // v.PluginName();
      // v.AuthorName();
      // v.UsesAddressLibrary();
      // v.UsesUpdatedStructs();
      // v.CompatibleVersions({SKSE::RUNTIME_SSE_LATEST});
      // v.MinimumRequiredXSEVersion(REL::Version{2, 3, 0});
      return data;
    }();