# Hello

A minimal SKSE plugin template for Skyrim Special Edition, built with CMake,
vcpkg, and CommonLibSSE.

## Features

- CMake + Ninja presets for MSVC and clang-cl, Debug and Release
- vcpkg manifest mode with `commonlib-shared` overlay
- `clangd` / `clang-tidy` / `clang-format` configured out of the box
- Static-linked runtime (`x64-windows-static-md`) — output is a single DLL
- Plugin metadata (name, author, version) generated from CMake

## Prerequisites

| Tool          | Version       | Notes                                         |
| ------------- | ------------- | --------------------------------------------- |
| Visual Studio | 2022 or newer | "Desktop development with C++" workload       |
| CMake         | 3.20+         | 4.x recommended                               |
| Ninja         | any           | bundled with VS or install separately         |
| vcpkg         | latest        | must set `VCPKG_ROOT`                         |
| LLVM          | 18+           | optional, only for clang-cl preset and clangd |
| Git           | any           | for submodules                                |

### Set `VCPKG_ROOT`

The build system reads `VCPKG_ROOT` from the environment.

```powershell
[Environment]::SetEnvironmentVariable(
    "VCPKG_ROOT",
    "C:\path\to\vcpkg",
    "User"
)
```

Reopen your terminal after setting it. Verify:

```powershell
echo $env:VCPKG_ROOT
```

## Getting Started

### 1. Clone with submodules

```powershell
git clone --recurse-submodules <repo-url> hello
cd hello
```

If you already cloned without `--recurse-submodules`:

```powershell
git submodule update --init --recursive
```

### 2. Configure

```powershell
cmake --preset msvc-debug
```

First run downloads and builds dependencies via vcpkg. Expect a few minutes.

### 3. Build

```powershell
cmake --build --preset msvc-debug
```

Output: `build/msvc-debug/Hello.dll`

### 4. Install

Copy the DLL into your Skyrim SE plugins directory:

```powershell
$plugins = "C:\SteamLibrary\steamapps\common\Skyrim Special Edition\Data\SKSE\Plugins"
Copy-Item .\build\msvc-debug\Hello.dll $plugins
```

### 5. Run

Launch Skyrim SE via `skse64_loader.exe`. Check logs:

- `Documents\My Games\Skyrim Special Edition\SKSE\skse64.log` — loader log
- `Documents\My Games\Skyrim Special Edition\SKSE\Hello.log` — plugin log

If the plugin loaded correctly, `skse64.log` shows:

```
loading plugin "Hello"
plugin Hello.dll (00000001 Hello 00010000) loaded correctly
```

## Presets

| Preset          | Compiler | Build type |
| --------------- | -------- | ---------- |
| `msvc-debug`    | MSVC     | Debug      |
| `msvc-release`  | MSVC     | Release    |
| `clang-debug`   | clang-cl | Debug      |
| `clang-release` | clang-cl | Release    |

Switch compilers by switching presets. Each preset has its own build directory
under `build/`.

## Customization

### Change plugin metadata

Edit `CMakeLists.txt`:

```cmake
project(Hello VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_AUTHOR "Your Name")
```

These values generate `build/<preset>/src/Plugin.h` and are used by
`SKSEPlugin_Version` in `src/main.cpp`.

### Add source files

```cmake
add_library("${PROJECT_NAME}" SHARED
    src/main.cpp
    src/foo.cpp
    src/bar.cpp
)
```

### Add dependencies

Edit `vcpkg.json`:

```json
{
  "name": "hello",
  "version": "0.1.0",
  "dependencies": ["commonlib-shared", "fmt"]
}
```

Then re-run configure. CMake links it:

```cmake
find_package(fmt CONFIG REQUIRED)
target_link_libraries("${PROJECT_NAME}" PRIVATE fmt::fmt)
```

## Editor Setup

### VS Code

Requires the [clangd extension](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd).

1. Configure `clang-debug` at least once so `compile_commands.json` exists:

   ```powershell
   cmake --preset clang-debug
   ```

2. Open the project folder. `clangd` starts indexing automatically.

3. Adjust `clangd.path` in `.vscode/settings.json` if LLVM isn't on `PATH`:

   ```json
   "clangd.path": "C:/path/to/llvm/bin/clangd.exe"
   ```

### Other editors

Any editor that understands `compile_commands.json` works. Point it at
`build/clang-debug/compile_commands.json`.

## Project Layout

```
.
├── .vscode/
│   └── settings.json          # clangd, format-on-save, file associations
├── cmake/
│   └── Plugin.h.in            # metadata template
├── extern/
│   └── CommonLibSSE/          # submodule
├── src/
│   ├── main.cpp               # SKSE entry points
│   └── pch.h                  # precompiled header
├── .clang-format
├── .clang-tidy
├── CMakeLists.txt
├── CMakePresets.json
├── vcpkg.json
└── vcpkg-configuration.json
```

## Troubleshooting

### "Could not find toolchain file"

`VCPKG_ROOT` isn't set. See [Prerequisites](#set-vcpkg_root).

### "error 126" in `skse64.log`

The plugin can't load a dependency DLL. This template uses
`x64-windows-static-md`, so the output should be self-contained. If you
switched triplets, either revert or copy the required DLLs next to
`Hello.dll`.

### clangd reports errors in `Plugin.h.in`

Add to `.vscode/settings.json`:

```json
"files.associations": {
    "*.h.in": "plaintext"
}
```

### clangd reports `@PROJECT_VERSION_MAJOR@` errors

Same as above — the `.in` file is a CMake template, not C++. Mark it as
`plaintext` so clangd ignores it.

### Build is slow on first configure

vcpkg compiles all dependencies from source. Subsequent configures reuse the
binary cache at `%LOCALAPPDATA%\vcpkg\archives`.

## License

This project is licensed under the **GNU General Public License v3.0 or later**.

It links against [CommonLibSSE](https://github.com/powerof3/CommonLibSSE),
which is GPL-3.0. Under GPL's copyleft terms, the resulting plugin must also
be distributed under a GPL-3.0-compatible license, with source code available
to anyone who receives the binary.

If you fork this template, keep the GPL-3.0 license or choose another
GPL-3.0-compatible one (e.g. AGPL-3.0). **Do not relicense it as MIT** —
that would violate the terms you inherited from CommonLibSSE.
