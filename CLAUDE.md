# CLAUDE.md - AI Assistant Guide for OpenS100

This document provides comprehensive guidance for AI assistants working with the OpenS100 codebase. It covers architecture, workflows, conventions, and best practices.

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [Repository Structure](#repository-structure)
3. [Architecture & Design Patterns](#architecture--design-patterns)
4. [Build System & Dependencies](#build-system--dependencies)
5. [Development Workflows](#development-workflows)
6. [Key Modules & Components](#key-modules--components)
7. [Coding Conventions](#coding-conventions)
8. [Testing Guidelines](#testing-guidelines)
9. [Common Tasks](#common-tasks)
10. [Important Files Reference](#important-files-reference)
11. [Troubleshooting](#troubleshooting)
12. [AI Assistant Best Practices](#ai-assistant-best-practices)

---

## Project Overview

**OpenS100** is an open-source IHO S-100 Electronic Navigational Chart (ENC) Viewer developed based on the KHOA S-100 Viewer. It's a Windows desktop application built with C++ and MFC for viewing and managing hydrographic and oceanographic data products.

### Key Facts
- **Language**: C++ (C++17+)
- **UI Framework**: Microsoft Foundation Classes (MFC)
- **Graphics**: Direct2D (D2D1) for high-performance rendering
- **Scripting**: Lua 5.3+ for feature portrayal
- **Platform**: Windows 10+, x64 only
- **Build Tool**: Visual Studio 2022
- **Package Manager**: vcpkg (manifest mode)
- **License**: Mozilla Public License version 2.0 (MPLv2)

### Supported S-100 Products
- **S-101** - Electronic Navigational Chart 2.0.0 (PRIMARY)
- **S-102** - Bathymetric Surface (test stage)
- **S-125** - Marine Protected Areas
- **S-201** - Inland ENC
- **S-111** - Surface Currents
- Additional: S-122, S-124, S-126, S-127, S-128, S-129, S-247, S-413, S-414, S-421, S-910, S-911, S-999

---

## Repository Structure

```
OpenS100/
├── OpenS100/              # Main MFC application (58 files)
│   ├── OpenS100App.cpp    # Application entry point
│   ├── MainFrm.cpp        # Main window frame
│   ├── OpenS100View.cpp   # Map rendering view
│   └── OpenS100Doc.cpp    # Document model
│
├── GISLibrary/            # Core GIS engine (707 files - LARGEST MODULE)
│   ├── LayerManager.*     # Multi-layer data management
│   ├── S101Cell.*         # S-101 ENC cell handling
│   ├── CatalogManager.*   # Feature/portrayal catalog coordination
│   ├── D2D1Resources.*    # Direct2D resource management
│   └── PortrayalCatalogue.* # Portrayal rule management
│
├── S100Engine/            # Rendering & standards engine (92 files)
│   ├── S100Render.cpp     # Main rendering engine
│   ├── S100PCManager.cpp  # Portrayal catalog management
│   └── SVGReader.cpp      # SVG symbol parsing
│
├── PortrayalCatalogue/    # Visual representation (254 files)
│   ├── Drawing instruction primitives
│   ├── Text, symbol, line, area styles
│   └── Template execution for feature portrayal
│
├── FeatureCatalog/        # Data definitions (135 files)
│   ├── FeatureCatalogue.h # Main feature catalog interface
│   └── Attribute definitions and visibility rules
│
├── GeoMetryLibrary/       # Geometry operations (32 files)
│   ├── Scaler.h           # Coordinate scaling/projection
│   ├── MBR.h              # Minimum Bounding Rectangle
│   └── ENCCommon.h        # Common ENC structures
│
├── LatLonUtility/         # Lat/Lon utilities (3 files)
├── LibMFCUtil/            # MFC utility library (8 files)
├── LuaScriptingReference/ # Lua integration (24 files)
│
├── ProgramData/           # Data files & catalogs (2,841 files)
│   ├── FC/                # Feature Catalog XMLs (S-101, S-125, etc.)
│   ├── PC/                # Portrayal Catalogs
│   │   ├── Symbols/       # 500+ SVG symbols
│   │   ├── Rules/         # XSLT portrayal rules
│   │   ├── ColorProfiles/ # Day/dusk/night color schemes
│   │   └── Fonts/         # TrueType fonts
│   └── World/             # Background map data
│
├── extlibs/               # External libraries
│   └── Lua/               # Lua 5.3+ implementation
│
├── Documents/             # Documentation
├── License/               # Third-party licenses
├── Redist/                # Redistribution scripts
│
├── OpenS100.sln           # Visual Studio solution
├── vcpkg.json             # vcpkg dependency manifest
├── vcpkg-configuration.json # vcpkg binary cache config
├── Doxyfile               # Doxygen configuration
├── README.md              # Project overview
├── DOXYGEN.md             # Documentation guide
└── vcpkg.md               # Dependency management guide
```

### Project Organization (9 Visual C++ Projects)

**Static Libraries (01. LIB):**
1. `FeatureCatalog` - Feature type definitions
2. `GeoMetryLibrary` - Geometry operations
3. `GISLibrary` - Core GIS engine (largest)
4. `LatLonUtility` - Geographic utilities
5. `S100Engine` - Rendering engine
6. `PortrayalCatalogue` - Visual representation

**Dynamic Libraries (02. DLL):**
7. `LibMFCUtil` - MFC utilities
8. `LuaScriptingReference` - Lua-C++ bridge

**Executable (03. EXE):**
9. `OpenS100` - Main application

---

## Architecture & Design Patterns

### Layered Architecture

```
┌─────────────────────────────────────┐
│  User Interface (MFC)               │ OpenS100/
│  - COpenS100App, CMainFrm           │
│  - COpenS100View, COpenS100Doc      │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│  GIS Core (GISLibrary)              │ GISLibrary/
│  - LayerManager                     │
│  - CatalogManager                   │
│  - S101Cell, S102Layer              │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│  Rendering Engine (S100Engine)      │ S100Engine/
│  - S100Render                       │
│  - Direct2D resource management     │
│  - Portrayal catalog compilation    │
└──────────────┬──────────────────────┘
               │
┌──────────────▼──────────────────────┐
│  Geometry & Math                    │ GeoMetryLibrary/
│  - Coordinate transformations       │
│  - Spatial calculations             │
└─────────────────────────────────────┘
```

### Design Patterns Used

1. **Document/View Pattern** (MFC standard)
   - `COpenS100Doc` - Data model
   - `COpenS100View` - Visual representation
   - Automatic update propagation

2. **Manager Pattern**
   - `LayerManager` - Multi-layer data management
   - `CatalogManager` - Feature/portrayal catalog coordination
   - `D2D1Resources` - Direct2D resource lifecycle

3. **Factory Pattern**
   - Feature creation from XML
   - Geometry instantiation
   - Portrayal instruction generation

4. **Observer Pattern**
   - Layer/document change notifications
   - View updates on data changes

5. **Strategy Pattern**
   - Different rendering strategies per feature type
   - Lua script-based portrayal customization

### Data Flow

```
ENC File (.S101/.GML)
    ↓
GISLibrary (parsing)
    ↓
LayerManager (organization)
    ↓
┌───────────────────────────────────┐
│ Portrayal Pipeline:               │
│  1. Feature Catalog (definitions) │
│  2. Portrayal Catalog (rules)     │
│  3. XSLT Rules (transformations)  │
│  4. Lua Scripts (custom logic)    │
│  5. SVG Symbols (graphics)        │
│  6. Color Profiles (themes)       │
└───────────────┬───────────────────┘
                ↓
        S100Render (Direct2D)
                ↓
        Screen Output
```

---

## Build System & Dependencies

### Prerequisites

1. **Visual Studio 2022** (v17.5+)
   - Workload: Desktop development with C++
   - Component: MFC for latest build tools
   - Platform: x64 (primary)

2. **vcpkg** (C++ package manager)
   - Manifest mode (dependencies in `vcpkg.json`)
   - Binary cache recommended

3. **Windows 10+** (x64)

### vcpkg Dependencies

```json
{
  "pugixml": "Fast XML parsing",
  "geographiclib": "Geographic calculations & projections",
  "polyclipping": "Polygon clipping operations",
  "hdf5": "Bathymetric data (S-102)",
  "libxslt": "XSLT transformation for portrayal rules",
  "openssl": "Security & encryption",
  "sqlite3": "Dataset metadata storage",
  "gdal": "Geospatial data abstraction",
  "boost-geometry": "Advanced geometry operations"
}
```

### Build Instructions

#### Quick Start

```bash
# 1. Set up vcpkg binary cache (optional but recommended)
$env:VCPKG_DEFAULT_BINARY_CACHE = "D:\vcpkg-cache"
[System.Environment]::SetEnvironmentVariable('VCPKG_DEFAULT_BINARY_CACHE','D:\vcpkg-cache','User')

# 2. Restart Visual Studio

# 3. Open solution
start OpenS100.sln

# 4. Set configuration
#    - Platform: x64
#    - Configuration: Debug or Release
#    - Startup Project: OpenS100

# 5. Build (Ctrl+Shift+B)
#    - vcpkg will auto-restore dependencies from vcpkg.json
#    - All 9 projects will build in dependency order
```

#### Manual vcpkg Installation

```bash
# Install vcpkg packages manually (if needed)
cd <vcpkg_root>
.\vcpkg install --triplet x64-windows

# Integrate with Visual Studio
.\vcpkg integrate install
```

### Build Configurations

| Configuration | Platform | Output Directory | Purpose |
|---------------|----------|------------------|---------|
| Debug | x64 | `Debug_x64/` | Development & debugging |
| Release | x64 | `Release_x64/` | Production builds |
| Debug | ARM64 | `Debug_ARM64/` | ARM64 testing |
| Release | ARM64 | `Release_ARM64/` | ARM64 production |

### Build Output

After successful build:
```
Release_x64/
├── OpenS100.exe           # Main executable
├── LibMFCUtil.dll         # MFC utilities
├── LuaScriptingReference.dll # Lua bridge
├── *.lib                  # Static libraries
├── *.pdb                  # Debug symbols
└── [vcpkg dependencies]   # Runtime DLLs
```

---

## Development Workflows

### Setting Up Development Environment

1. **Clone Repository**
   ```bash
   git clone <repository_url>
   cd OpenS100
   ```

2. **Configure vcpkg Cache** (see Build Instructions above)

3. **Open in Visual Studio**
   - Open `OpenS100.sln`
   - Wait for IntelliSense indexing

4. **Verify Build**
   - Build solution (Ctrl+Shift+B)
   - Set OpenS100 as startup project
   - Run (F5)

### Common Development Scenarios

#### Adding New Feature Support

1. **Locate Feature Catalog**
   - Check `ProgramData/FC/` for S-XXX_FC.xml
   - Understand feature types and attributes

2. **Implement in GISLibrary**
   - Add feature class in `GISLibrary/` (similar to `S101Cell.*`)
   - Implement parsing logic
   - Add to `LayerManager`

3. **Add Portrayal Support**
   - Create portrayal catalog in `ProgramData/PC/S-XXX_Portrayal/`
   - Add XSLT rules in `Rules/` subfolder
   - Define symbols in `Symbols/` (SVG format)

4. **Update UI**
   - Modify dialogs in `OpenS100/` for user interaction
   - Update menu/toolbar if needed

#### Modifying Rendering

1. **Direct2D Changes**
   - Core rendering: `S100Engine/S100Render.cpp`
   - Resource management: `GISLibrary/D2D1Resources.*`

2. **Portrayal Logic**
   - Portrayal rules: `ProgramData/PC/*/Rules/*.xsl`
   - Lua scripts: Custom logic in `LuaScriptingReference/`

3. **Symbol Updates**
   - SVG symbols: `ProgramData/PC/*/Symbols/`
   - Ensure valid SVG 1.1 format

#### Adding Geometry Operations

1. **Core Geometry**
   - Implement in `GeoMetryLibrary/`
   - Follow existing patterns (Point, Polygon, Polyline)

2. **Integration**
   - Use in `GISLibrary/` for spatial operations
   - Update `Scaler.*` for coordinate transformations

### Git Workflow

```bash
# Create feature branch
git checkout -b feature/my-feature

# Make changes and commit
git add .
git commit -m "Add S-XXX product support"

# Push to remote
git push -u origin feature/my-feature

# Create pull request (via GitHub web UI)
```

### Testing Changes

1. **Manual Testing**
   - Run application (F5 in Visual Studio)
   - Open test ENC data
   - Verify rendering and functionality

2. **Unit Testing**
   - Use `TestGISLibrary.*` framework
   - Add test cases for new features

3. **Data Testing**
   - Test with sample S-100 datasets
   - Verify compliance with IHO standards

---

## Key Modules & Components

### OpenS100 (Main Application)

**Purpose**: MFC-based user interface and application lifecycle

**Key Files**:
- `OpenS100/OpenS100App.cpp:77` - `InitInstance()` - Application entry point
- `OpenS100/MainFrm.cpp` - Main window, menu, toolbar management
- `OpenS100/OpenS100View.cpp` - Map canvas, rendering, user input
- `OpenS100/OpenS100Doc.cpp` - Document model, data management

**Important Classes**:
- `COpenS100App` - Application singleton
- `CMainFrm` - Main window frame
- `COpenS100View` - Map view (handles rendering)
- `COpenS100Doc` - Document (data model)

**Common Tasks**:
- Add menu items: Edit `MainFrm.cpp`, resource `OpenS100.rc`
- Modify view behavior: Edit `OpenS100View.cpp`
- Handle file operations: Edit `OpenS100Doc.cpp`

### GISLibrary (Core Engine)

**Purpose**: Core GIS data processing and management (707 files)

**Critical Components**:

1. **Layer Management**
   - `LayerManager.*` - Multi-layer coordination
   - `S101Cell.*` - S-101 ENC cell data
   - `S102Layer.*` - Bathymetric surfaces

2. **Catalog Management**
   - `CatalogManager.*` - Feature/portrayal catalog coordination
   - `PortrayalCatalogue.*` - Portrayal rules
   - `ExchangeCatalogue.*` - Data interchange

3. **Rendering Support**
   - `D2D1Resources.*` - Direct2D resource lifecycle
   - `S100EditRender.*` - Edit mode rendering

4. **Geometry**
   - `Point.*`, `Polygon.*`, `Polyline.*` - Basic shapes
   - `MultiPoint.*`, `Surface.*` - Complex geometries
   - `ATTR.*` - Attribute handling

5. **Data Structures**
   - `SpatialReference.*` - Coordinate systems
   - `VerticalDatumReference.*` - Vertical datums
   - `MBR.h` - Minimum Bounding Rectangles (spatial indexing)

**Architecture Pattern**:
```cpp
// Typical usage pattern
LayerManager* manager = new LayerManager();
S101Cell* cell = new S101Cell();
cell->LoadFromFile(L"path/to/dataset.S101");
manager->AddLayer(cell);
manager->Render(pD2DDeviceContext);
```

### S100Engine (Rendering Engine)

**Purpose**: S-100 standard compliance and rendering (92 files)

**Key Files**:
- `S100Render.cpp` - Main rendering operations
- `S100PCManager.cpp` - Portrayal catalog management
- `S100ColorProfile.cpp` - Color scheme handling
- `SVGReader.cpp` - SVG symbol parsing

**Important Classes**:
- `S100Render` - Core rendering logic
- `S100PCManager` - Portrayal catalog compilation
- `LineStyle`, `CompositeLineStyle` - Line rendering
- `Circle`, `Dash`, `Line` - Drawing primitives

**Rendering Pipeline**:
```
Feature → Portrayal Rules (XSLT) → Drawing Instructions → Direct2D → Screen
```

### PortrayalCatalogue (Visual Representation)

**Purpose**: Manage how features are visually represented (254 files)

**Components**:
- Drawing instruction primitives
- Text styling and layout
- Symbol definitions and placement
- Line, area, and point styles
- Template execution

**Key Enums** (200+ S-100 standard classes):
- Display priority
- Drawing order
- Viewing groups
- Style properties

### FeatureCatalog (Data Definitions)

**Purpose**: Define and manage feature types and attributes (135 files)

**Key Files**:
- `FeatureCatalogue.h` - Main interface
- Feature type enumerations
- Attribute definitions
- Association definitions
- ISO 19115 metadata structures

**Usage**:
```cpp
// Load feature catalog
FeatureCatalogue fc;
fc.LoadFromXML(L"ProgramData/FC/S-101_FC.xml");

// Query feature definition
FeatureType* ft = fc.GetFeatureType("Buoy");
```

### GeoMetryLibrary (Geometry Operations)

**Purpose**: Core geometric operations and transformations (32 files)

**Key Files**:
- `Scaler.h` - Coordinate scaling and projection
- `MBR.h` - Minimum Bounding Rectangle operations
- `ENCCommon.h` - Common ENC structures
- `GeoCommonFuc.h` - Utility functions

**Common Operations**:
- Coordinate transformations (lat/lon ↔ screen)
- Bounding box calculations
- Spatial indexing support

### LuaScriptingReference (Lua Integration)

**Purpose**: Expose C++ functionality to Lua scripts (24 files)

**Key Files**:
- `lua_session.*` - Lua VM management
- `lua_reference.*` - C++-Lua object binding
- `lua_functions.cpp` - Exported functions
- Spatial types: Point, Surface, CompositeCurve, MultiPoint
- `Result_DrawingInstruction.h` - Drawing instruction results

**Usage in Portrayal**:
```lua
-- Example Lua script for portrayal
function portray_buoy(feature)
    local symbol = CreateSymbol("BOYSPP")
    symbol:SetPosition(feature.geometry.point)
    return symbol
end
```

---

## Coding Conventions

### File Organization

1. **Header Files (.h)**
   ```cpp
   #pragma once

   // Include guards (if not using #pragma once)
   #ifndef CLASSNAME_H
   #define CLASSNAME_H

   // Standard library includes
   #include <string>
   #include <vector>

   // Third-party includes
   #include "pugixml.hpp"

   // Project includes
   #include "../GeoMetryLibrary/ENCCommon.h"

   // Class declaration
   class ClassName {
   public:
       // Public interface

   protected:
       // Protected members

   private:
       // Private implementation
   };

   #endif // CLASSNAME_H (if using include guards)
   ```

2. **Implementation Files (.cpp)**
   ```cpp
   #include "pch.h"  // Precompiled header (MFC projects)
   #include "ClassName.h"

   // Implementation
   ```

### Naming Conventions

| Type | Convention | Example |
|------|------------|---------|
| **Classes** | PascalCase (C prefix for MFC) | `LayerManager`, `COpenS100App` |
| **Member Variables** | camelCase or m_ prefix | `layerCount`, `m_pDocument` |
| **Functions/Methods** | PascalCase (MFC) or camelCase | `GetLayerCount()`, `render()` |
| **Constants** | UPPER_SNAKE_CASE or k prefix | `MAX_LAYERS`, `kDefaultZoom` |
| **Namespaces** | lowercase | `namespace s100 { }` |
| **Enums** | PascalCase (members UPPER) | `enum class DisplayPriority { HIGH, MEDIUM, LOW }` |
| **Files** | PascalCase matching class | `LayerManager.h`, `LayerManager.cpp` |

### Memory Management

1. **Prefer Smart Pointers** (modern code)
   ```cpp
   std::unique_ptr<LayerManager> manager = std::make_unique<LayerManager>();
   std::shared_ptr<Feature> feature = std::make_shared<Feature>();
   ```

2. **Legacy MFC Code** (existing patterns)
   ```cpp
   CLayerManager* manager = new CLayerManager();
   // ... use ...
   delete manager;  // Don't forget!
   ```

3. **COM Objects** (Direct2D)
   ```cpp
   ID2D1RenderTarget* pRenderTarget = nullptr;
   // ... create ...
   if (pRenderTarget) {
       pRenderTarget->Release();
       pRenderTarget = nullptr;
   }
   ```

### Error Handling

1. **Return Codes** (MFC style)
   ```cpp
   BOOL LoadData(const CString& filePath) {
       if (!ValidateFile(filePath)) {
           AfxMessageBox(_T("Invalid file"));
           return FALSE;
       }
       return TRUE;
   }
   ```

2. **Exceptions** (modern C++)
   ```cpp
   void ParseXML(const std::string& xml) {
       try {
           pugi::xml_document doc;
           if (!doc.load_string(xml.c_str())) {
               throw std::runtime_error("Failed to parse XML");
           }
       } catch (const std::exception& e) {
           // Handle error
       }
   }
   ```

### String Handling

1. **MFC Strings** (UI code)
   ```cpp
   CString message = _T("Hello, World!");  // Unicode-safe
   ```

2. **Standard Strings** (core logic)
   ```cpp
   std::string utf8String = "Hello";
   std::wstring wideString = L"Hello";
   ```

3. **Conversions**
   ```cpp
   // CString → std::string
   CStringA utf8(cstring);
   std::string str(utf8);

   // std::string → CString
   CString cstr(str.c_str());
   ```

### Direct2D Resource Management

```cpp
class D2D1Resources {
public:
    HRESULT CreateResources(ID2D1RenderTarget* pRT) {
        HRESULT hr = S_OK;

        // Create brush
        if (SUCCEEDED(hr)) {
            hr = pRT->CreateSolidColorBrush(
                D2D1::ColorF(D2D1::ColorF::Black),
                &m_pBlackBrush
            );
        }

        return hr;
    }

    void DiscardResources() {
        SafeRelease(&m_pBlackBrush);
    }

private:
    ID2D1SolidColorBrush* m_pBlackBrush = nullptr;

    template<class Interface>
    void SafeRelease(Interface** ppInterfaceToRelease) {
        if (*ppInterfaceToRelease != nullptr) {
            (*ppInterfaceToRelease)->Release();
            *ppInterfaceToRelease = nullptr;
        }
    }
};
```

---

## Testing Guidelines

### Unit Testing

**Location**: `OpenS100/TestGISLibrary.*`

**Pattern**:
```cpp
class TestGISLibrary {
public:
    void RunAllTests() {
        TestLayerManager();
        TestS101Parsing();
        TestGeometryOperations();
    }

private:
    void TestLayerManager() {
        LayerManager manager;
        // Assertions
        ASSERT(manager.GetLayerCount() == 0);
    }
};
```

### Manual Testing Checklist

1. **Data Loading**
   - [ ] Load S-101 dataset
   - [ ] Load S-102 bathymetric data
   - [ ] Load multiple datasets simultaneously
   - [ ] Verify proper layer organization

2. **Rendering**
   - [ ] Pan and zoom operations
   - [ ] Symbol rendering correctness
   - [ ] Color profile switching (day/dusk/night)
   - [ ] Text label placement

3. **Feature Interaction**
   - [ ] Feature selection
   - [ ] Attribute display
   - [ ] Feature editing (if applicable)

4. **Performance**
   - [ ] Large dataset loading time
   - [ ] Smooth panning/zooming
   - [ ] Memory usage monitoring
   - [ ] Rendering frame rate

### Test Data

**Location**: Not in repository (see `.gitignore`)
- `ENCData/` - Test ENC datasets (gitignored)
- `SampleData/` - Sample data files (gitignored)

**Acquire Test Data**:
- Official IHO S-100 test datasets
- KHOA sample data
- Generate using `S101Creator` class

---

## Common Tasks

### Task 1: Add New Menu Item

1. **Edit Resource File**
   ```cpp
   // OpenS100/OpenS100.rc
   // Add menu item in resource editor
   ID_NEW_COMMAND  "New Command Description"
   ```

2. **Add Command Handler**
   ```cpp
   // MainFrm.h
   class CMainFrm : public CFrameWndEx {
       afx_msg void OnNewCommand();
       DECLARE_MESSAGE_MAP()
   };

   // MainFrm.cpp
   BEGIN_MESSAGE_MAP(CMainFrm, CFrameWndEx)
       ON_COMMAND(ID_NEW_COMMAND, &CMainFrm::OnNewCommand)
   END_MESSAGE_MAP()

   void CMainFrm::OnNewCommand() {
       // Implementation
   }
   ```

### Task 2: Load and Display ENC Data

```cpp
// In COpenS100Doc::OnOpenDocument()
BOOL COpenS100Doc::OnOpenDocument(LPCTSTR lpszPathName) {
    // Get GIS library instance
    auto* gisLib = theApp.GetGISLibrary();

    // Create S-101 cell
    S101Cell* cell = new S101Cell();

    // Load from file
    if (!cell->LoadFromFile(lpszPathName)) {
        delete cell;
        return FALSE;
    }

    // Add to layer manager
    gisLib->GetLayerManager()->AddLayer(cell);

    // Update views
    UpdateAllViews(NULL);

    return TRUE;
}
```

### Task 3: Custom Rendering

```cpp
// In COpenS100View::OnDraw()
void COpenS100View::OnDraw(CDC* pDC) {
    COpenS100Doc* pDoc = GetDocument();

    // Get Direct2D render target from DC
    ID2D1RenderTarget* pRT = GetRenderTarget(pDC);

    pRT->BeginDraw();

    // Clear background
    pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // Render layers
    auto* gisLib = theApp.GetGISLibrary();
    gisLib->Render(pRT, GetViewExtent());

    HRESULT hr = pRT->EndDraw();
    if (hr == D2DERR_RECREATE_TARGET) {
        DiscardDeviceResources();
    }
}
```

### Task 4: Add Lua Scripting Function

```cpp
// LuaScriptingReference/lua_functions.cpp
int lua_CustomFunction(lua_State* L) {
    // Get parameters from Lua stack
    double param1 = lua_tonumber(L, 1);
    const char* param2 = lua_tostring(L, 2);

    // Perform operation
    double result = DoSomething(param1, param2);

    // Return result to Lua
    lua_pushnumber(L, result);
    return 1;  // Number of return values
}

// Register function
static const luaL_Reg functions[] = {
    {"CustomFunction", lua_CustomFunction},
    {NULL, NULL}
};

void RegisterCustomFunctions(lua_State* L) {
    luaL_setfuncs(L, functions, 0);
}
```

### Task 5: Add New Geometry Type

```cpp
// GeoMetryLibrary/MyGeometry.h
#pragma once
#include "ENCCommon.h"

class MyGeometry {
public:
    MyGeometry();
    ~MyGeometry();

    void SetPoints(const std::vector<POINT_D>& points);
    MBR GetBoundingBox() const;
    bool Intersects(const MyGeometry& other) const;

private:
    std::vector<POINT_D> m_points;
    MBR m_boundingBox;
};
```

---

## Important Files Reference

### Configuration Files

| File | Purpose | When to Modify |
|------|---------|----------------|
| `OpenS100.sln` | Solution configuration | Adding new projects |
| `*/*.vcxproj` | Project build settings | Changing compiler options, dependencies |
| `vcpkg.json` | Package dependencies | Adding new libraries |
| `vcpkg-configuration.json` | vcpkg cache settings | Setting up binary cache |
| `Doxyfile` | Documentation generation | Customizing docs output |
| `.gitignore` | Git exclusions | Excluding new build artifacts |

### Data Files

| File | Purpose | Format |
|------|---------|--------|
| `ProgramData/FC/S-101_FC.xml` | S-101 Feature Catalog | XML |
| `ProgramData/PC/S-101_Portrayal/portrayal_catalogue.xml` | S-101 Portrayal Catalog | XML |
| `ProgramData/PC/*/ColorProfiles/colorProfile.xml` | Color schemes | XML |
| `ProgramData/PC/*/Rules/*.xsl` | Portrayal transformation rules | XSLT |
| `ProgramData/PC/*/Symbols/*.svg` | Feature symbols | SVG 1.1 |

### Entry Points

| File | Purpose | Key Function |
|------|---------|--------------|
| `OpenS100/OpenS100App.cpp` | Application entry | `InitInstance():77` |
| `OpenS100/MainFrm.cpp` | Main window creation | `OnCreate()` |
| `OpenS100/OpenS100View.cpp` | Rendering entry | `OnDraw()` |
| `GISLibrary/LayerManager.cpp` | Layer rendering | `Render()` |
| `S100Engine/S100Render.cpp` | Core rendering | `RenderFeature()` |

---

## Troubleshooting

### Build Issues

#### Problem: "Cannot find vcpkg packages"
**Solution**:
```bash
# Ensure vcpkg is integrated
cd <vcpkg_root>
.\vcpkg integrate install

# Restore packages
.\vcpkg install --triplet x64-windows
```

#### Problem: "MFC libraries not found"
**Solution**:
- Open Visual Studio Installer
- Modify VS 2022 installation
- Add "MFC and ATL support (x64)"

#### Problem: "Direct2D headers missing"
**Solution**:
- Install Windows SDK (10.0.19041.0 or later)
- Verify include paths in project properties

### Runtime Issues

#### Problem: "Application fails to start - missing DLLs"
**Solution**:
```bash
# Copy vcpkg DLLs to output directory
# Or run from Redist/OpenS100.bat to package dependencies
```

#### Problem: "Feature Catalog not found"
**Solution**:
- Ensure `ProgramData/` directory is copied to executable location
- Check working directory in debugger settings
- Verify XML file paths in code

#### Problem: "Symbols not rendering"
**Solution**:
- Check SVG symbol files in `ProgramData/PC/*/Symbols/`
- Verify portrayal catalog XML references correct symbols
- Check XSLT rule execution in debugger

### Performance Issues

#### Problem: "Slow rendering with large datasets"
**Solutions**:
1. Enable spatial indexing (MBR-based)
2. Implement view frustum culling
3. Use Direct2D hardware acceleration
4. Optimize symbol caching

#### Problem: "High memory usage"
**Solutions**:
1. Implement lazy loading for large datasets
2. Release unused Direct2D resources
3. Use memory pooling for frequent allocations
4. Profile with Visual Studio Performance Profiler

---

## AI Assistant Best Practices

### When Working with This Codebase

1. **Always Check Module Boundaries**
   - Respect the layered architecture
   - Don't create circular dependencies
   - UI code stays in `OpenS100/`
   - Core logic stays in `GISLibrary/`
   - Rendering stays in `S100Engine/`

2. **Follow Existing Patterns**
   - Look for similar implementations before creating new ones
   - Example: Adding S-XXX support? Study `S101Cell.*` first
   - Maintain consistency with existing code style

3. **Respect Standards Compliance**
   - This is an IHO S-100 compliant viewer
   - Changes must not break S-100 standard compliance
   - Consult S-100 specifications when in doubt

4. **Memory Safety**
   - Be cautious with raw pointers (legacy MFC code)
   - Always match `new` with `delete`
   - Release COM objects (Direct2D) with `->Release()`
   - Consider smart pointers for new code

5. **Unicode Awareness**
   - Use `_T()` macro for string literals in MFC code
   - Use `CString` for UI, `std::wstring` for wide chars
   - Be careful with conversions

6. **Direct2D Resources**
   - Always release resources in reverse creation order
   - Handle `D2DERR_RECREATE_TARGET` errors
   - Don't hold resources across frames unnecessarily

7. **Testing**
   - Test with actual S-100 data files
   - Verify rendering with different color profiles
   - Check performance with large datasets

8. **Documentation**
   - Add Doxygen comments for new classes/functions
   - Update CLAUDE.md when making architectural changes
   - Keep README.md in sync with features

### Code Review Checklist

Before committing changes, verify:

- [ ] Code compiles without warnings (x64 Release)
- [ ] No memory leaks (check with Visual Studio debugger)
- [ ] Direct2D resources properly released
- [ ] Follows naming conventions
- [ ] Includes Doxygen documentation
- [ ] Tested with sample S-100 data
- [ ] No hardcoded paths (use relative paths)
- [ ] Unicode-safe string handling
- [ ] Error handling implemented
- [ ] No introduction of circular dependencies

### Common Pitfalls to Avoid

1. **Don't Mix MFC and STL Inappropriately**
   ```cpp
   // Bad: Mixing in public interfaces
   class MyClass {
   public:
       std::vector<CString> GetItems();  // Confusing mix
   };

   // Good: Keep consistent within modules
   class MyClass {
   public:
       CArray<CString> GetItems();  // MFC-style
   };
   ```

2. **Don't Forget vcpkg Dependencies**
   - Always update `vcpkg.json` when adding new libraries
   - Don't commit vcpkg_installed/ directory

3. **Don't Hardcode Paths**
   ```cpp
   // Bad
   LoadFile("C:\\Data\\sample.S101");

   // Good
   CString appPath = GetModuleDirectory();
   LoadFile(appPath + "\\ProgramData\\sample.S101");
   ```

4. **Don't Ignore S-100 Standards**
   - Feature catalogs define valid feature types
   - Portrayal catalogs define rendering rules
   - Don't create custom features without updating catalogs

5. **Don't Skip Resource Cleanup**
   ```cpp
   // Bad
   ID2D1Brush* pBrush = CreateBrush();
   // ... forgot to release ...

   // Good
   ID2D1Brush* pBrush = CreateBrush();
   // ... use ...
   SafeRelease(&pBrush);
   ```

### Useful Debugging Tips

1. **Direct2D Debugging**
   - Enable D2D debug layer in Debug builds
   - Check return codes from all D2D calls
   - Use PIX or Graphics Diagnostics for rendering issues

2. **XML Parsing Issues**
   - Use pugixml error reporting
   - Validate XML against S-100 schemas
   - Check file encoding (UTF-8)

3. **Lua Scripting Issues**
   - Check Lua stack balance
   - Use `lua_gettop()` to verify stack state
   - Enable Lua error messages

4. **Memory Debugging**
   ```cpp
   // Enable memory leak detection
   #ifdef _DEBUG
   #define _CRTDBG_MAP_ALLOC
   #include <crtdbg.h>
   #define new DEBUG_NEW
   #endif

   // At program exit
   _CrtDumpMemoryLeaks();
   ```

### Quick Reference: File Location by Task

| Task | Primary Files |
|------|---------------|
| Add menu command | `OpenS100/MainFrm.*`, `OpenS100/OpenS100.rc` |
| Modify rendering | `S100Engine/S100Render.cpp`, `GISLibrary/LayerManager.cpp` |
| Add geometry type | `GeoMetryLibrary/` |
| Parse new S-100 product | `GISLibrary/` (new S-XXX class) |
| Add Lua function | `LuaScriptingReference/lua_functions.cpp` |
| Customize portrayal | `ProgramData/PC/*/Rules/*.xsl` |
| Add symbol | `ProgramData/PC/*/Symbols/` (SVG) |
| Modify color scheme | `ProgramData/PC/*/ColorProfiles/colorProfile.xml` |
| Update feature definitions | `ProgramData/FC/*.xml` |
| Add dependency | `vcpkg.json` |

---

## Additional Resources

### Documentation
- **IHO S-100 Standards**: https://iho.int/en/s-100-universal-hydrographic-data-model
- **Direct2D API**: https://docs.microsoft.com/en-us/windows/win32/direct2d/
- **MFC Documentation**: https://docs.microsoft.com/en-us/cpp/mfc/
- **Lua 5.3 Manual**: https://www.lua.org/manual/5.3/
- **vcpkg Guide**: https://vcpkg.io/

### Generate API Documentation
```bash
# Install Doxygen
choco install doxygen.install

# Generate docs
doxygen Doxyfile

# Open documentation
start docs/html/index.html
```

### Project Participants
- [Korea Hydrographic and Oceanographic Agency (KHOA)](https://www.khoa.go.kr/eng/)
- [Korea Research Institute of Ships and Ocean (KRISO)](https://www.kriso.re.kr/eng/)
- THE PROST Inc. (until 2021)
- [BLUEMAP Inc.](https://bluemap.kr)

---

**Last Updated**: 2025-11-19
**Document Version**: 1.0
**OpenS100 Version**: Based on latest development branch

For questions or clarifications, refer to existing code patterns or consult the S-100 standards documentation.
