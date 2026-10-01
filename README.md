# Hello

基于 CMake、vcpkg 和 CommonLibSSE 的极简 SKSE 插件模板。

## ✨ 特性

- **构建**：CMake Presets + Ninja，支持 MSVC (`msvc-*`) 与 Clang (`clang-*`)。
- **依赖**：vcpkg manifest 模式，静态链接（`x64-windows-static-md`），开箱即用。
- **代码质量**：内置 `clangd`、`clang-tidy`、`clang-format` 配置。
- **MO2 集成**：检测到 `$env:MO2` 环境变量时，编译后自动复制 DLL/PDB 到模组目录。

## 🛠️ 环境要求

| 工具          | 要求                                        |
| ------------- | ------------------------------------------- |
| Visual Studio | 2022+（含 C++ 桌面开发工作负载）            |
| CMake         | 3.20+                                       |
| vcpkg         | 最新版，需设置 `VCPKG_ROOT` 环境变量        |
| LLVM          | 18+（可选，供 clangd 和 clang-cl 预设使用） |

> ⚠️ 首次使用请确保终端能读到 `$env:VCPKG_ROOT`。

## 🚀 快速开始

```powershell
# 1. 拉取代码与子模块
git clone --recurse-submodules <repo-url> hello
cd hello

# 2. 配置并编译（首次会编译 vcpkg 依赖，耗时较长）
cmake --preset msvc-debug
cmake --build --preset msvc-debug
```

输出文件位于：`build/msvc-debug/Hello.dll`

### 自动部署到 MO2（可选）

设置环境变量 `MO2` 指向 MO2 根目录：

```powershell
[Environment]::SetEnvironmentVariable("MO2", "C:\path\to\ModOrganizer-2.5.2", "User")
```

重新打开终端后，每次编译成功，DLL 和 PDB 会自动复制到 `mods/Hello/SKSE/Plugins/` 下。

## 💻 编辑器配置 (VS Code)

本项目使用 `clangd` 提供代码补全，**无需**微软 C/C++ 扩展。

首次克隆后，必须生成一次 clang 索引（因为 clangd 无法解析 MSVC 的编译参数）：

```powershell
cmake --preset clang-debug
```

之后打开 VS Code，clangd 会自动读取 `build/clang-debug/compile_commands.json` 并开始索引。

> 💡 **提示**：写代码时使用 `clang-debug` 预设保证 clangd 正常运作；实际调试运行用 `msvc-debug` 预设，两者互不干扰。

## 📂 项目结构

```
.
├── .vscode/settings.json      # clangd 与格式化配置
├── cmake/Plugin.h.in          # 插件元数据模板
├── extern/CommonLibSSE/       # Git 子模块
├── src/
│   ├── main.cpp               # SKSE 入口点
│   └── pch.h                  # 预编译头
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
└── vcpkg-configuration.json
```

## 📝 自定义

修改 `CMakeLists.txt` 顶部的项目信息：

```cmake
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Charlene Hoo")
set(PROJECT_AUTHOR_EMAIL "CharleneHoo@hotmail.com")
```

## 📄 License

GPL-3.0-or-later。

本项目链接了 GPL-3.0 的 [CommonLibSSE](https://github.com/powerof3/CommonLibSSE)。根据 GPL 传染性条款，衍生插件必须同样以 GPL-3.0 兼容协议开源。**请勿改为 MIT 协议**。
