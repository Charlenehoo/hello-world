// src/Debug/DecapitateTest.h

#pragma once

namespace Debug {
// 对准星目标尝试 NGD 斩首。仅用于开发调试。
// 会在日志里打完整过程：目标识别、NGD 可用性、调用结果。
void DecapitateCrosshairTarget();
}  // namespace Debug