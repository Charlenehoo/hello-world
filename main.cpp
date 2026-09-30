#include "pch.h" // IWYU pragma: keep

extern "C" [[maybe_unused]] __declspec(dllexport) auto
SKSEPlugin_Load(const SKSE::LoadInterface *skse) -> bool {
  return true;
}