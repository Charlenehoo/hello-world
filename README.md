# Hello

基于 CMake、vcpkg 和 [CommonLibSSE](https://github.com/powerof3/CommonLibSSE) 的极简 SKSE 插件模板。

本模板的默认工作流经过维护者验证。照做就行。等你熟悉了，你自然会知道什么时候该改。

## ✨ 特性

- CMake Presets 管理 MSVC / Clang 构建
- vcpkg manifest 管理依赖
- 集成 CommonLibSSE，支持 Skyrim AE
- 自带事件处理示例：
  - `TESEquipEvent`
  - `TESObjectLoadedEvent`
  - `InputEvent`
- 集成 clang-format / clang-tidy
- VS Code + clangd
- MO2 自动部署
- SKSE 日志自动链接到项目根目录

## 📦 环境要求

全部装好，缺一不可。

- Windows 10/11 x64
- Visual Studio 2022，勾选“使用 C++ 的桌面开发”
  - MSVC v143
  - Windows SDK
  - C++ Clang tools for Windows
- CMake 3.31+（必须能识别 `CMakePresets.json` 的 `version: 10`）
- Ninja
- Git
- vcpkg，并设置 `VCPKG_ROOT`
- Skyrim Special Edition / AE
- SKSE64 2.3.0+
- Address Library for SKSE Plugins（AE 版）
- VS Code
- `llvm-vs-code-extensions.vscode-clangd`
- MO2

## 🚀 快速开始

### 1. 准备 vcpkg

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg
C:\dev\vcpkg\bootstrap-vcpkg.bat
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
```

设置完必须重开终端。

### 2. 装 clang-cl 和 clangd 扩展

Visual Studio Installer → 修改 → 单个组件 → 勾选：

```text
C++ Clang tools for Windows
```

VS Code 扩展市场搜：

```text
llvm-vs-code-extensions.vscode-clangd
```

装上。

### 3. 克隆项目

```powershell
git clone --recurse-submodules "https://github.com/Charlenehoo/hello-world.git"
cd hello-world
```

忘了子模块就补：

```powershell
git submodule update --init --recursive
```

### 4. 打开 Developer PowerShell for VS 2022

从开始菜单打开，不要用普通 PowerShell。

原因：`CMakePresets.json` 的 MSVC 预设依赖 `$env{VCToolsInstallDir}`。普通 PowerShell 里这个变量是空的，配置直接失败。

### 5. 先配置 clang-debug

即使日常用 MSVC 编译，这一步也不能省。clangd 只认它。

```powershell
cmake --preset clang-debug
```

### 6. 配置并构建

```powershell
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

首次配置会编译 vcpkg 依赖，耗时很长，等着。

输出：

```text
build/msvc-debug/Hello.dll
```

## 🛠 构建预设

| Preset          | 编译器   | 构建类型 | 输出目录              |
| --------------- | -------- | -------- | --------------------- |
| `msvc-debug`    | MSVC     | Debug    | `build/msvc-debug`    |
| `msvc-release`  | MSVC     | Release  | `build/msvc-release`  |
| `clang-debug`   | clang-cl | Debug    | `build/clang-debug`   |
| `clang-release` | clang-cl | Release  | `build/clang-release` |

日常用 `msvc-debug`，发布用 `msvc-release`。

`clang-*` 预设的职责是给 clangd 喂编译数据库。插件由 `msvc-*` 编译产出。

```powershell
cmake --preset msvc-debug
cmake --build --preset msvc-debug

cmake --preset msvc-release
cmake --build --preset msvc-release
```

## 🧠 clangd

固定流程：

```powershell
# 1. 喂 clangd（每次改完 CMake 相关配置都要跑）
cmake --preset clang-debug

# 2. 实际构建
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

两者互不干扰，可以共存。

### 硬性规则

1. `clang-cl` 必装。没它就没有 `build/clang-debug/compile_commands.json`，clangd 索引不到任何东西。
2. `llvm-vs-code-extensions.vscode-clangd` 必装。这是官方扩展，模板按它配置。
3. `.vscode/settings.json` 里 `--compile-commands-dir` 指向 `build/clang-debug`。模板已经写死：

```json
"--compile-commands-dir=${workspaceFolder}/build/clang-debug"
```

原因：`cl.exe` 生成的 `compile_commands.json` 里塞满了 clangd 解析不了的东西：

- MSVC 专有 flag：`/Zc:__cplusplus`、`/permissive-`、`/utf-8`、`/std:c++23`
- Windows SDK 和 MSVC STL 头文件里的非标准扩展、内部宏、`#pragma`
- MSVC 工具链特有的隐式定义和路径

clangd 读 MSVC 的编译数据库会满屏“找不到符号”“宏未定义”“头文件解析失败”，索引基本报废。所以 clangd 只看 `build/clang-debug`。

### 检查清单

- [ ] Visual Studio Installer 里勾了 `C++ Clang tools for Windows`
- [ ] VS Code 装了 `llvm-vs-code-extensions.vscode-clangd`
- [ ] `build/clang-debug/compile_commands.json` 存在
- [ ] `.vscode/settings.json` 里的 `--compile-commands-dir` 是 `build/clang-debug`
- [ ] `C_Cpp.intelliSenseEngine` 是 `"disabled"`

全勾上，VS Code 才算配置完。

## 📂 项目结构

```text
.
├─ CMakeLists.txt
├─ CMakePresets.json
├─ vcpkg.json
├─ vcpkg-configuration.json
├─ .clang-format
├─ .clang-tidy
├─ cmake/
│  ├─ .clangd.in
│  └─ Plugin.h.in
├─ extern/
│  └─ CommonLibSSE/
├─ src/
│  ├─ main.cpp
│  ├─ pch.h
│  └─ Event/
│     ├─ EventProcessor.cpp
│     ├─ EventProcessor.h
│     ├─ InputProcessor.cpp
│     └─ InputProcessor.h
└─ .vscode/
   └─ settings.json
```

`cmake/Plugin.h.in` 在构建目录生成 `src/Plugin.h`，含版本、插件名、作者信息。

## 🎮 部署

用 MO2。

### MO2 自动部署

配置之前设置环境变量：

```powershell
$env:MO2 = "D:\MO2"
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

插件会被复制到：

```text
D:\MO2\mods\Hello\SKSE\Plugins\
```

Debug 构建还会自动复制 PDB。

注意：

- `MO2` 指向 MO2 根目录，不是 `mods` 目录
- 该检查在 CMake 配置阶段执行，设完变量必须重新 `cmake --preset`
- 持久化：`[Environment]::SetEnvironmentVariable("MO2", "D:\MO2", "User")`

## 📝 日志

SKSE 日志：

```text
Documents/My Games/Skyrim Special Edition/SKSE/Hello.log
```

`CMakeLists.txt` 会把它硬链接到项目根：

```text
Hello.log
```

如果配置时日志还不存在，会提示 `SKSE log not found`。跑一次游戏，再重新配置：

```powershell
cmake --preset msvc-debug
```

跨卷时自动退化为复制。

## 📄 自定义

改 `CMakeLists.txt` 顶部：

```cmake
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Charlene Hoo")
set(PROJECT_AUTHOR_EMAIL "CharleneHoo@hotmail.com")
```

改项目名后，以下全部跟着变：

- 输出 DLL 名
- MO2 自动复制目录
- SKSE 日志文件名
- 根目录日志链接名

同步改：

- `vcpkg.json` 里的 `"name"`（保持小写）
- `README.md` 标题
- clone URL
- 需要的话改 `vcpkg.json` 的 `"version"`

## ❓ 常见问题

### `VCPKG_ROOT` 未设置

```powershell
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
```

重开终端。

### `VCToolsInstallDir` 为空

用 Developer PowerShell for VS 2022。

### CommonLibSSE 缺失

```powershell
git submodule update --init --recursive
```

### 首次构建很慢

正常。vcpkg 在编 CommonLibSSE。等着。

### clangd 找不到 `compile_commands.json`

```powershell
cmake --preset clang-debug
```

### clangd 满屏误报

按顺序排查：

1. 装没装 `C++ Clang tools for Windows`？没装去装。
2. 装没装 `llvm-vs-code-extensions.vscode-clangd`？没装去装。
3. `--compile-commands-dir` 是不是指向 `build/clang-debug`？不是就改成它。
4. `build/clang-debug/compile_commands.json` 生成了吗？没有就 `cmake --preset clang-debug`。
5. `C_Cpp.intelliSenseEngine` 是不是 `"disabled"`？不是就关掉它。

### clang 预设失败

Visual Studio Installer → 修改 → 单个组件 → `C++ Clang tools for Windows`。

### 游戏里没加载插件

逐条排查：

- SKSE 2.3.0+
- Address Library for SKSE Plugins 已装
- DLL 在 `Data/SKSE/Plugins/` 或 MO2 的对应位置
- 看日志：`Documents/My Games/Skyrim Special Edition/SKSE/Hello.log`

### MO2 自动复制没生效

```powershell
echo $env:MO2
```

必须是 MO2 根目录。设完重开终端，重新 `cmake --preset`。

## 📜 License

GPL-3.0-or-later。完整协议见 `LICENSE`。

本项目链接了 GPL-3.0 的 [CommonLibSSE](https://github.com/powerof3/CommonLibSSE)。根据 GPL 传染性条款，衍生插件必须同样以 GPL-3.0 兼容协议开源。

**请勿改为 MIT 协议。**

## 👤 关于作者

Charlene Hoo  
[CharleneHoo@hotmail.com](mailto:CharleneHoo@hotmail.com)

## 🙏 致谢

- [CommonLibSSE](https://github.com/powerof3/CommonLibSSE)
- [SKSE](https://skse.silverlock.org/)
- [vcpkg](https://github.com/microsoft/vcpkg)
- [CMake](https://cmake.org/)
- [clangd](https://clangd.llvm.org/)
