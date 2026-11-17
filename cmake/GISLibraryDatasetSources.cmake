# How the sources of GISLibrary are shared out between the package targets (file names without
# ".cpp").
#
# GISLibrary still holds the dataset model, the readers, the portrayal run and the Windows display
# code in one directory; the Visual Studio project builds all of it.
#   opens100::dataset    every source except the ones listed in this file
#   opens100::portrayal  OPENS100_GISLIBRARY_PORTRAYAL_SOURCES
#   (no target)          display, display clients, System Database, other
#
# Each target is closed: dataset alone, and portrayal on top of dataset and luascripting, link
# with --whole-archive and no undefined symbol.

# opens100::portrayal - what is to be drawn: the drawing instruction model (SENC_*), the manager
# that builds it from the output of the rules, the Lua host that answers the rules from an S-101
# dataset, the run itself (Lua 5.1 for S-101, XSLT 1.0 for the other products), and the
# instructions as JSON text (DisplayListJson). DisplayListPainter is the S-100 side of drawing
# them on an IRenderer; it is here until a portable backend gives it a target of its own.
set(OPENS100_GISLIBRARY_PORTRAYAL_SOURCES
    DisplayListJson DisplayListPainter PCOutputSchemaManager ProcessS101 S100LayerBuildPortrayal S100SpatialObjectPortrayal
    S101LuaScriptingReference host_data host_functions
    SENC_AreaInstruction SENC_AugmentedPath SENC_AugmentedRay SENC_Color SENC_ColorFill
    SENC_ColorProfile SENC_CompositeLineStyle SENC_DisplayList SENC_Element SENC_Font SENC_Hatch
    SENC_HatchFill SENC_Instruction SENC_LineInstruction SENC_LineStyle SENC_LineStyleBase
    SENC_LineStyleReference SENC_NullInstruction SENC_OverrideColor SENC_Pen SENC_Pixmap
    SENC_PointInstruction SENC_SimpleLineStyle SENC_Symbol SENC_SymbolFill SENC_Template
    SENC_TextInstruction SENC_TextPoint SENC_pixmapFill)

# Draws with Direct2D or GDI, or builds their resources. In no package target until a portable
# render backend replaces them (Documents/TechDocs/2026-11-17-render-separation-design.md).
set(OPENS100_GISLIBRARY_DISPLAY_SOURCES
    CoverageDataset D2D1Resources D2DGeometryCache D2DInstructionCache D2DInstructionPainter
    D2DRenderer
    DepthSuppression ExchangeCatalogue GISLibrary GdiGeometryRenderer Inventory InventoryItem
    LayerManager LayerOpen PortrayalRenderCache S100EditRender S100ExchangeCatalogue
    S100LayerPortrayal S100Utilities SHPFile)

# Refers to the display code above: layers copied for editing, a product's portrayal catalogue
# with the Direct2D resource store, the set of layers shown from the System Database.
set(OPENS100_GISLIBRARY_DISPLAY_CLIENT_SOURCES
    DatasetLayerSet Product S101Factory)

# Needs the System Database module (opens100::sdb).
set(OPENS100_GISLIBRARY_SDB_SOURCES
    CatalogManager SdbReaders SystemDatabaseChart SystemDatabaseSession)

# Base64 through OpenSSL; nothing in the dataset set calls it.
set(OPENS100_GISLIBRARY_OTHER_SOURCES
    Base64String)

set(OPENS100_GISLIBRARY_NON_DATASET_SOURCES
    ${OPENS100_GISLIBRARY_PORTRAYAL_SOURCES}
    ${OPENS100_GISLIBRARY_DISPLAY_SOURCES}
    ${OPENS100_GISLIBRARY_DISPLAY_CLIENT_SOURCES}
    ${OPENS100_GISLIBRARY_SDB_SOURCES}
    ${OPENS100_GISLIBRARY_OTHER_SOURCES})
