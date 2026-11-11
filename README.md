# OpenS100

**An open-source C++ library for IHO S-100 data, with a reference Windows viewer.**

[![License: MPL 2.0](https://img.shields.io/badge/License-MPL_2.0-brightgreen.svg)](LICENSE.txt)
![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![S-100 Edition 5.2](https://img.shields.io/badge/S--100-Edition_5.2-0a7ea4.svg)
![Platform: Windows x64](https://img.shields.io/badge/platform-Windows_x64-lightgrey.svg)

OpenS100 reads S-100 datasets (ISO 8211, GML, HDF5), loads feature and portrayal catalogues, and draws
them with the S-100 Part 9 portrayal engine. It started as the KHOA S-100 Viewer and is being reshaped
into a library that any application can build on: add it with vcpkg, link a CMake target, and start
reading S-100 data.

![OpenS100 viewer](https://github.com/user-attachments/assets/d3b5e499-860d-4bba-b09d-a7f4339525b7)

## Use it in your project

OpenS100 ships a vcpkg port and CMake package. A complete, working consumer is in
[examples/consumer](examples/consumer).

**1. Point vcpkg at the port** (`vcpkg-configuration.json`, path to your OpenS100 checkout):

```json
{ "overlay-ports": ["path/to/OpenS100/ports"] }
```

**2. Depend on it** (`vcpkg.json`):

```json
{ "dependencies": ["opens100"] }
```

**3. Link the modules you need** (`CMakeLists.txt`):

```cmake
find_package(opens100 CONFIG REQUIRED)
target_link_libraries(app PRIVATE opens100::featurecatalog opens100::geometry)
```

**4. Read an S-101 ENC:**

```cpp
#include <FeatureCatalog/FeatureCatalogue.h>
#include <GISLibrary/S101Cell.h>
#include <iostream>

int main()
{
    // The feature catalogue tells the reader what the dataset contains.
    FeatureCatalogue fc;
    if (!fc.Read(std::string("ProgramData/FC/S-101_FC.xml")))
        return 1;

    // Open an ISO 8211 cell. No window or renderer is needed.
    S101Cell cell(&fc);
    if (!cell.Open(L"SampleData/101KR005X01NE.000"))
        return 1;

    std::cout << cell.GetFeatureCount() << " features, "
              << cell.GetCount_CurveRecord() << " curves, "
              << cell.GetCount_SurfaceRecord() << " surfaces\n";
}
```

> `S101Cell` is in GISLibrary, which is built by `OpenS100.sln` today and is not in the vcpkg package
> yet. [examples/consumer](examples/consumer) shows what links from the package right now: it reads a
> feature catalogue with `opens100::featurecatalog`.

### Modules

Each module is a separate CMake target, so you only pull in the dependencies you use.

| Target | vcpkg feature | What it does | Depends on |
|---|---|---|---|
| `opens100::latlon` | (base) | Geodesy and coordinate utilities | GeographicLib |
| `opens100::geometry` | `geometry` (default) | Geometry primitives, map scaler, display settings | |
| `opens100::featurecatalog` | `feature-catalogue` (default) | S-100 Part 5 feature catalogue model and reader | pugixml, utfcpp |
| `opens100::coverage` | `coverage` | S-100 Part 10c HDF5 coverages, depth lines, water level adjustment | HDF5 |
| `opens100::sdb` | `system-database` | Local store of installed datasets and catalogues, S-98 dataset selection, S-100 Part 15 data protection | SQLite, OpenSSL, zlib, pugixml |

The dataset readers (ISO 8211, GML) and the portrayal engine are built by the Visual Studio solution
today and are joining the CMake package as they are separated from MFC and Direct2D.

## What it supports

OpenS100 is catalogue-driven: a product specification is supported through its feature catalogue and
portrayal catalogue rather than through product-specific code. It targets **S-100 Edition 5.2** and the
ECDIS behaviour described in **S-98**.

| Area | Status |
|---|---|
| S-101 ENC (ISO 8211, Lua portrayal) | Supported |
| S-102 Bathymetric Surface (HDF5), with the S-98 enhanced safety contour | Supported |
| S-104 Water Level, with S-98 water level adjustment of S-102 | Supported |
| GML products with XSLT portrayal (S-122 to S-131, S-201, S-4xx, ...) | Supported in the viewer |
| S-111 Surface Currents | Regular grids are read; display is in progress |
| S-100 Part 15 data protection (encrypted and signed exchange sets) | Supported |
| S-100 Part 17 exchange sets and the S-98 dataset loading algorithm | Supported |

Feature and portrayal catalogues for 27 product specifications are included in
[ProgramData](ProgramData), among them S-101, S-102, S-104, S-111, S-122 to S-131, S-201, S-411 to
S-414 and S-421.

## Build and run the viewer

Requirements:

- Windows 10 or later, x64
- Visual Studio 2026 (v145 toolset, C++20) with the **Desktop development with C++** workload and **MFC**
- [vcpkg](https://vcpkg.io/) (dependencies are restored automatically from `vcpkg.json`)

Steps:

1. Clone the repository and open `OpenS100.sln`.
2. Select the `x64` platform and set `OpenS100` as the startup project.
3. Build and run.

[BUILD.md](BUILD.md) has the detailed, verified setup, including the command-line build and the
recommended vcpkg binary cache.

To try it right away, open a dataset from [SampleData](SampleData). It contains S-101 cells, S-102
bathymetry and GML datasets (S-127, S-129, S-201).

## Tests

The GoogleTest suite in [Tests/OpenS100Tests](Tests/OpenS100Tests) has about 170 tests covering the
dataset readers, catalogues, coverages, the system database, data protection and off-screen rendering.

```bat
Debug_x64\OpenS100Tests.exe
```

Tests that need data not shipped in the repository are skipped automatically.

## Documentation

- [BUILD.md](BUILD.md): build setup
- [CONVENTIONS.md](CONVENTIONS.md): coding conventions
- [DOXYGEN.md](DOXYGEN.md): generating the API reference
- [OSS Notice.md](OSS%20Notice.md): third-party licenses

## Contributing

Issues and pull requests are welcome at
[S-100ExpertTeam/OpenS100](https://github.com/S-100ExpertTeam/OpenS100). Please follow
[CONVENTIONS.md](CONVENTIONS.md) and add a test for what you change.

## License

OpenS100 is released under the [Mozilla Public License 2.0](LICENSE.txt).

## Project participants

- [Korea Hydrographic and Oceanographic Agency (KHOA)](https://www.khoa.go.kr/eng/)
- [Korea Research Institute of Ships and Ocean Engineering (KRISO)](https://www.kriso.re.kr/eng/)
- THE PROST Inc. (until 2021)
- [BLUEMAP Inc.](https://bluemap.kr)

## References

- [IHO S-100 Universal Hydrographic Data Model](https://iho.int/en/s-100-universal-hydrographic-data-model)
- [Digital Incubator](https://digitalincubator.maritimeconnectivity.net/framework/)
