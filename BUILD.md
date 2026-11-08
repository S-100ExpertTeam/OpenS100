# Build Guide (Windows / Visual Studio / vcpkg)

This document describes a **stable and verified build setup** for Windows using  
Visual Studio + vcpkg (manifest mode).

It is based on real build issues encountered during dependency installation
(notably `pugixml`) and documents the **recommended configuration** that avoids
archive extraction and CMake toolchain problems.

---

## 1. Prerequisites

### 1.1 Operating System
- Windows 10 / 11 (x64)

### 1.2 Visual Studio
- Visual Studio 2026 (version 18). The projects use the **v145** toolset and **C++20** (`/std:c++20`).
- Required workload:
  - **Desktop development with C++**
- Required components:
  - MSVC v145 toolset
  - **MFC for v145** (C++ MFC for latest build tools). The MFC projects fail with MSB8041 if this is missing.
  - Windows 10/11 SDK

> Visual Studio 2022 (v143) is no longer the supported toolset since 2026-11. If you must stay on v143, change `PlatformToolset` in each `.vcxproj` and install MFC for v143.

---

## 2. CMake Configuration (Important)

### 2.1 Required CMake Version
- **CMake 3.29.x**

> Newer CMake versions bundled with vcpkg (e.g. 3.31.x) may cause
> archive extraction failures on Windows during `vcpkg install`
> (for example when extracting `.tar.gz` sources such as `pugixml`).

### 2.2 Install CMake
Download and install CMake 3.29 from:
- https://cmake.org/download/

After installation, verify:
```bat
cmake --version
```

---

## 3. vcpkg Setup

### 3.1 vcpkg Location

This guide assumes:

```text
C:\vcpkg
```

Make sure vcpkg is up to date:

```bat
cd C:\vcpkg
git pull
vcpkg update
```

---

## 4. Force vcpkg to Use System CMake

To avoid issues with the CMake version bundled inside vcpkg,
**force vcpkg to use the system-installed CMake**.

### 4.1 Environment Variable

Set the following environment variable:

```bat
VCPKG_FORCE_SYSTEM_BINARIES=1
```

This ensures:

* vcpkg does **not** use its internally downloaded CMake
* the system-installed CMake 3.29 is used instead

> After setting this variable, **restart Visual Studio and all terminals**.

---

## 5. Recommended vcpkg Binary Cache Configuration

Using a binary cache is strongly recommended to:

* reduce build time
* avoid repeated source extraction
* improve build stability (especially for manifest mode)

### 5.1 Recommended Setting

Set the following environment variable:

```bat
VCPKG_BINARY_SOURCES=clear;files,C:\vcpkg\binary-cache,readwrite
```

### 5.2 Notes

* `clear` ensures no implicit or legacy cache sources are used
* `files,C:\vcpkg\binary-cache,readwrite` enables a local binary cache
* This setting works consistently for:

  * CLI builds
  * Visual Studio builds
  * vcpkg manifest mode

### 5.3 Important

If `VCPKG_BINARY_SOURCES` is set,
`VCPKG_DEFAULT_BINARY_CACHE` is **not required and is ignored**.

---

## 6. Build Workflow

### 6.1 Clean Previous State (Optional but Recommended)

If you experienced dependency build failures before:

```bat
rmdir /s /q C:\vcpkg\buildtrees
```

(Do **not** delete `binary-cache`.)

---

### 6.2 Build from the command line

```bat
"C:\Program Files\Microsoft Visual Studio8\Community\MSBuild\Current\Bin\MSBuild.exe" OpenS100.sln -m -p:Configuration=Debug -p:Platform=x64
Debug_x64\OpenS100Tests.exe
```

The second line runs the GoogleTest suite (`Tests/OpenS100Tests`). Tests that need
reference data read it from `S100_TDS_ROOT` (default `D:\s100-reference	ds`) and skip
when the files are absent.

### 6.3 Build from Visual Studio

* Open the solution
* Select configuration:

  * `Debug | x64` or `Release | x64`
* Build the project

vcpkg dependencies will be:

* restored from binary cache when available
* built using system CMake when required

---

## 7. Summary of Required Settings

| Item                          | Value                                         |
| ----------------------------- | --------------------------------------------- |
| Visual Studio                 | **2026 (v145), C++20, MFC for v145**           |
| CMake                         | **3.29.x (system installed)**                 |
| vcpkg mode                    | Manifest                                      |
| `VCPKG_FORCE_SYSTEM_BINARIES` | `1`                                           |
| `VCPKG_BINARY_SOURCES`        | `clear;files,C:\vcpkg\binary-cache,readwrite` |
| Binary cache                  | Enabled (recommended)                         |

---

## 8. Rationale

This configuration avoids:

* CMake archive extraction issues on Windows
* differences between CLI and Visual Studio builds
* repeated source builds of common libraries (e.g. `pugixml`)

It has been validated in real-world builds and is recommended
as the **baseline Windows build configuration**.

---
