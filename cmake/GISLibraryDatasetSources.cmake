# Sources of GISLibrary that are NOT part of opens100::dataset (file names without ".cpp").
#
# GISLibrary still holds the dataset model, the readers and the Windows display code in one
# directory. opens100::dataset is every source in it except the ones listed here; the Visual
# Studio project builds all of them. The list shrinks as drawing moves behind the render backend
# (Documents/TechDocs/2026-11-08-phase1-core-decoupling.md, item 5-3).
#
# The remaining set is closed: it links with --whole-archive and no undefined symbol.

# Draws with Direct2D or GDI, or builds their resources.
set(OPENS100_GISLIBRARY_DISPLAY_SOURCES
    CoverageDataset D2D1Resources D2DGeometryCache DepthSuppression ExchangeCatalogue GISLibrary
    GdiGeometryRenderer Inventory InventoryItem LayerManager LayerOpen PCOutputSchemaManager
    PortrayalRenderCache S100EditRender S100ExchangeCatalogue S100LayerPortrayal
    S100SpatialObjectPortrayal S100Utilities SHPFile
    SENC_AreaInstruction SENC_AugmentedPath SENC_AugmentedRay SENC_Color SENC_ColorPalette SENC_ColorFill
    SENC_ColorProfile SENC_CompositeLineStyle SENC_DisplayList SENC_Element SENC_Font SENC_Hatch
    SENC_HatchFill SENC_Instruction SENC_LineInstruction SENC_LineStyle SENC_LineStyleBase
    SENC_LineStyleReference SENC_NullInstruction SENC_OverrideColor SENC_Pen SENC_Pixmap
    SENC_PointInstruction SENC_SimpleLineStyle SENC_Symbol SENC_SymbolFill SENC_Template
    SENC_TextInstruction SENC_TextPoint SENC_pixmapFill)

# Refers to the display code above: layers copied for editing, a product's portrayal catalogue
# with the Direct2D resource store, the set of layers shown from the System Database.
set(OPENS100_GISLIBRARY_DISPLAY_CLIENT_SOURCES
    DatasetLayerSet Product S101Factory)

# Runs S-101 portrayal: the Lua host (Lua 5.1, LuaScriptingReference) and the XSLT processor.
set(OPENS100_GISLIBRARY_PORTRAYAL_SOURCES
    ProcessS101 S101LuaScriptingReference host_data host_functions)

# Needs the System Database module (opens100::sdb).
set(OPENS100_GISLIBRARY_SDB_SOURCES
    CatalogManager SdbReaders SystemDatabaseChart SystemDatabaseSession)

# Base64 through OpenSSL; nothing in the dataset set calls it.
set(OPENS100_GISLIBRARY_OTHER_SOURCES
    Base64String)

set(OPENS100_GISLIBRARY_NON_DATASET_SOURCES
    ${OPENS100_GISLIBRARY_DISPLAY_SOURCES}
    ${OPENS100_GISLIBRARY_DISPLAY_CLIENT_SOURCES}
    ${OPENS100_GISLIBRARY_PORTRAYAL_SOURCES}
    ${OPENS100_GISLIBRARY_SDB_SOURCES}
    ${OPENS100_GISLIBRARY_OTHER_SOURCES})
