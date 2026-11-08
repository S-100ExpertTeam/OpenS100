# OpenS100
OpenS100 is an open-source C++ library for IHO S-100 hydrographic data, with a reference Windows viewer.  
It was developed based on KHOA S-100 Viewer and is being reshaped from a viewer into a core library
(ISO 8211, GML, HDF5, feature/portrayal catalogues, Part 9/13 portrayal engine) that applications such as the
KHOA S-100 Viewer build on. See the plans in [Documents/TechDocs](Documents/TechDocs/README.md).  

# S-100 Product Specifications: Support Status
- S-101 Electronic Navigational Chart 2.0.0
- S-100 GML (test stage)
- S-102 Bathymetric Surface (test stage)

# Features
- Use Direct2D for performance. 
- Use source codes of LuaScriptingReference for S-101 PC.

# Requirements
- Microsoft Visual Studio 2026 (v145 toolset, C++20) with MFC
- MFC (Microsoft Foundation Class Library)
- Windows 10 (or later)
- Visual C++ Redistributable for Visual Studio 2012
- Only support x64.
- vcpkg

# Build instructions
- See [BUILD.md](BUILD.md) for the verified setup (vcpkg manifest mode, binary cache).
- Install vcpkg packages. ([Manual](vcpkg.md))
- Open OpenS100.sln.
- Select a plastform as x64.
- Set a project(OpenS100) as startup project. 
- Run.

# Main screen
![image](https://github.com/user-attachments/assets/d3b5e499-860d-4bba-b09d-a7f4339525b7)

# Tests
- `python tools/check-core-includes.py` reports how many core headers still name Direct2D/MFC/Win32 types (Phase 1 progress gauge).
- `Tests/OpenS100Tests` (GoogleTest). Build the solution and run `Debug_x64\OpenS100Tests.exe`.
- Reference data is read from `S100_TDS_ROOT` (default `D:\s100-reference	ds`); tests skip when it is absent.

# License
OpenS100 is released under the MPLv2 (Mozilla Public License version 2.0)

# Project participants
* [Korea Hydrographic and Oceanographic Agency (KHOA)](https://www.khoa.go.kr/eng/)
* [Korea Research Institute of Ships and Ocean (KRISO)](https://www.kriso.re.kr/eng/)
* THE PROST Inc. (~ 2021)
* [BLUEMAP Inc.](https://bluemap.kr)

# References
* [Digital Incubator](https://digitalincubator.maritimeconnectivity.net/framework/)
