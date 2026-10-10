#pragma once

namespace Debug {
// 生成一个与给定尸体完全重合的克隆 Actor。
//
// 前置条件：
//   - a_source 是 3D 已加载的 Actor（通常是已 ragdoll 稳定的尸体）
//
// 异步：等克隆体的 ragdoll 就绪后执行对齐，完成后回调（可为空）。
// 回调参数是生成的克隆体；失败时为 nullptr。
//
// 实验代码，已知限制：
//   - 不做存档序列化
//   - 不做两具 ragdoll 的碰撞隔离（它们会在下一帧互相弹开）
//   - 不做死亡音效抑制
void CloneAndOverlapStableCorpse(RE::Actor* a_source, std::function<void(RE::Actor*)> a_callback = nullptr);
}  // namespace Debug