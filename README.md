# Hello

基于 CMake、vcpkg 和 [CommonLibSSE](https://github.com/powerof3/CommonLibSSE) 的极简 SKSE 插件模板。

工作流经过维护者验证，推荐你按这条路径走。

## 📦 环境要求

这些建议都准备好，后面会顺很多。

- Windows 10/11 x64
- Visual Studio 2022（勾选“使用 C++ 的桌面开发”，含 **C++ Clang tools for Windows**）
- CMake 3.31+、Ninja、Git
- vcpkg，并设置 `VCPKG_ROOT`
- Skyrim SE / AE + SKSE64 2.3.0+ + Address Library（AE 版）
- VS Code + `llvm-vs-code-extensions.vscode-clangd`
- MO2

**clang-cl 和 clangd 扩展建议一定装上**，装了会舒服很多。

## 🚀 快速开始

```powershell
# 1. 准备 vcpkg
git clone https://github.com/microsoft/vcpkg.git C:\dev\vcpkg
C:\dev\vcpkg\bootstrap-vcpkg.bat
[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")
# 设完推荐重开一下终端

# 2. 克隆项目
git clone --recurse-submodules "https://github.com/Charlenehoo/hello-world.git"
cd hello-world

# 3. 推荐用 Developer PowerShell for VS 2022

# 4. 先喂 clangd
cmake --preset clang-debug

# 5. 再构建
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

输出：`build/msvc-debug/Hello.dll`

首次构建会编译 vcpkg 依赖，耗时会长一些，耐心等等就好。

## 🛠 构建预设

| Preset          | 用途                      |
| --------------- | ------------------------- |
| `msvc-debug`    | 日常开发（推荐默认）      |
| `msvc-release`  | 发布                      |
| `clang-debug`   | 喂 clangd，不用于产出插件 |
| `clang-release` | 同上                      |

`clang-*` 的职责是给 clangd 喂编译数据库，插件推荐由 `msvc-*` 产出。

## 🧠 clangd

推荐固定用这个流程：

```powershell
cmake --preset clang-debug   # 喂 clangd
cmake --preset msvc-debug    # 实际构建
cmake --build --preset msvc-debug
```

三条小建议：

1. `clang-cl` 推荐装上。
2. `llvm-vs-code-extensions.vscode-clangd` 推荐装上。
3. `--compile-commands-dir` 推荐指向 `build/clang-debug`（模板已经写好了）。

原因：`cl.exe` 生成的 `compile_commands.json` 里全是 clangd 解析不了的东西——MSVC 专有 flag、STL 内部扩展、隐式定义。喂给 clangd 容易满屏误报，索引体验会差很多。

## 🎮 部署

推荐用 MO2。配置前设置环境变量：

```powershell
$env:MO2 = "D:\MO2"
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

插件会自动复制到 `D:\MO2\mods\Hello\SKSE\Plugins\`，Debug 还会带上 PDB。

想持久化的话：`[Environment]::SetEnvironmentVariable("MO2", "D:\MO2", "User")`

## 📝 日志

```text
Documents/My Games/Skyrim Special Edition/SKSE/Hello.log
```

`CMakeLists.txt` 会把它硬链接到项目根的 `Hello.log`。如果配置时日志还不存在，跑一次游戏再重新 `cmake --preset msvc-debug` 就好。

## 📄 自定义

改 `CMakeLists.txt` 顶部：

```cmake
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Charlene Hoo")
set(PROJECT_AUTHOR_EMAIL "CharleneHoo@hotmail.com")
```

改项目名后，DLL 名、MO2 目录、日志名会同步变，`vcpkg.json` 里的 `"name"` 推荐保持小写同步改一下。

## ❓ 常见问题

**`VCPKG_ROOT` 未设置**  
推荐这样设一下：`[Environment]::SetEnvironmentVariable("VCPKG_ROOT", "C:\dev\vcpkg", "User")`，然后重开终端。

**`VCToolsInstallDir` 为空**  
换成 Developer PowerShell for VS 2022 就好。

**CommonLibSSE 缺失**  
`git submodule update --init --recursive`

**clangd 找不到 `compile_commands.json`**  
先跑一下 `cmake --preset clang-debug`。

**clangd 满屏误报**  
推荐检查四件事：装了 clang-cl 吗、装了 clangd 扩展吗、`--compile-commands-dir` 是不是 `build/clang-debug`、`C_Cpp.intelliSenseEngine` 是不是 `"disabled"`。

**游戏里没加载插件**  
推荐依次看：SKSE 版本、Address Library、DLL 位置，然后翻日志。

**MO2 自动复制没生效**  
`echo $env:MO2` 最好指向 MO2 根目录，设完重开终端重新 `cmake --preset` 一下。

## 📜 License

GPL-3.0-or-later，见 `LICENSE`。

本项目链接了 GPL-3.0 的 CommonLibSSE，衍生插件推荐同样以 GPL-3.0 兼容协议开源。**请勿改为 MIT。**

## 👤 关于作者

Charlene Hoo  
[CharleneHoo@hotmail.com](mailto:CharleneHoo@hotmail.com)
