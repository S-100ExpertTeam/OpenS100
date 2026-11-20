# OpenS100 - CMake build of the library modules that are already platform-neutral.
#
# This is the "walking skeleton" of roadmap Phase 4/5 (Documents/TechDocs/
# 2026-11-09-cmake-vcpkg-skeleton.md): configure -> build -> install -> find_package(opens100)
# -> vcpkg port -> an external consumer. Modules join this build as they lose their Windows-only
# dependencies; until then the Visual Studio solution (OpenS100.sln) remains the full build and
# both build the same source files.
#
#   cmake -S . -B build/cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
#   cmake --build build/cmake
#   cmake --install build/cmake --prefix build/install
cmake_minimum_required(VERSION 3.24)

project(opens100
    VERSION 0.2.0
    DESCRIPTION "Core C++ library for IHO S-100 hydrographic data"
    HOMEPAGE_URL "https://github.com/S-100ExpertTeam/OpenS100"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(OPENS100_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(OPENS100_BUILD_GEOMETRY "Build opens100::geometry (GeoMetryLibrary)" ON)
option(OPENS100_BUILD_FEATURECATALOG "Build opens100::featurecatalog (FeatureCatalog)" ON)
option(OPENS100_BUILD_PORTRAYALCATALOGUE "Build opens100::portrayalcatalogue (PortrayalCatalogue)" ON)
option(OPENS100_BUILD_SYSTEMDATABASE "Build opens100::sdb (SystemDatabase)" ON)

if(OPENS100_BUILD_PORTRAYALCATALOGUE AND NOT (OPENS100_BUILD_GEOMETRY AND OPENS100_BUILD_FEATURECATALOG))
    message(FATAL_ERROR "opens100::portrayalcatalogue needs opens100::geometry and opens100::featurecatalog")
endif()
option(OPENS100_BUILD_COVERAGE "Build opens100::coverage (Coverage)" ON)
option(OPENS100_BUILD_DATASET "Build opens100::dataset (GISLibrary: dataset model and ISO 8211, GML, HDF5 readers)" ON)
option(OPENS100_BUILD_LUASCRIPTING "Build opens100::luascripting (LuaScriptingReference: the Lua session of S-100 Part 9a portrayal)" ON)

option(OPENS100_BUILD_PORTRAYAL "Build opens100::portrayal (GISLibrary: drawing instruction model and the portrayal run)" ON)

if(OPENS100_BUILD_PORTRAYAL AND NOT (OPENS100_BUILD_DATASET AND OPENS100_BUILD_LUASCRIPTING))
    message(FATAL_ERROR "opens100::portrayal needs opens100::dataset and opens100::luascripting")
endif()
option(OPENS100_BUILD_RENDER_SKIA "Build opens100::render-skia (RenderSkia: the Skia backend of the renderer)" OFF)

if(OPENS100_BUILD_RENDER_SKIA AND NOT OPENS100_BUILD_PORTRAYAL)
    message(FATAL_ERROR "opens100::render-skia needs opens100::portrayal")
endif()
if(OPENS100_BUILD_LUASCRIPTING AND NOT OPENS100_BUILD_FEATURECATALOG)
    message(FATAL_ERROR "opens100::luascripting needs opens100::featurecatalog")
endif()
if(OPENS100_BUILD_DATASET AND NOT OPENS100_BUILD_PORTRAYALCATALOGUE)
    message(FATAL_ERROR "opens100::dataset needs opens100::portrayalcatalogue")
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(GeographicLib CONFIG REQUIRED)
if(OPENS100_BUILD_COVERAGE OR OPENS100_BUILD_DATASET)
    find_package(hdf5 CONFIG REQUIRED)
    # HDF5 is shared or static with the vcpkg triplet; link whichever one the package provides.
    if(TARGET hdf5::hdf5-shared)
        set(OPENS100_HDF5_TARGET hdf5::hdf5-shared)
    else()
        set(OPENS100_HDF5_TARGET hdf5::hdf5-static)
    endif()
endif()
if(OPENS100_BUILD_DATASET)
    find_package(pugixml CONFIG REQUIRED)
    find_package(polyclipping CONFIG REQUIRED)
    find_package(Boost REQUIRED)   # Boost.Geometry, header-only
endif()
if(OPENS100_BUILD_LUASCRIPTING)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    find_package(LuaJIT REQUIRED)
endif()
if(OPENS100_BUILD_PORTRAYAL)
    find_package(LibXml2 REQUIRED)
    find_package(LibXslt REQUIRED)   # XSLT 1.0 portrayal rules
endif()
if(OPENS100_BUILD_RENDER_SKIA)
    find_package(unofficial-skia CONFIG REQUIRED)
endif()
if(OPENS100_BUILD_FEATURECATALOG)
    find_package(pugixml CONFIG REQUIRED)
    find_package(utf8cpp CONFIG REQUIRED)
endif()
if(OPENS100_BUILD_SYSTEMDATABASE)
    find_package(unofficial-sqlite3 CONFIG REQUIRED)
    # S-100 Part 15 data protection (DataProtection, ZipArchive, ProtectedImport).
    find_package(pugixml CONFIG REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

# Headers are installed as <prefix>/include/opens100/<Module>/<header>, the same relative layout
# as the source tree, so the modules' "../Other/x.h" includes keep working after installation and
# consumers write  #include <FeatureCatalog/FeatureCatalogue.h>.
set(OPENS100_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/opens100")

# opens100_add_module(<target> <directory> [DEPENDS <targets>...]
#                     [EXCLUDE <source names>...] | [SOURCES <source names>...])
# A static library from a source directory: every *.cpp in it (the .vcxproj lists exactly those)
# except the EXCLUDE names, or only the SOURCES names (both without ".cpp"). A directory shared
# by two targets has its headers installed by the one without SOURCES.
function(opens100_add_module target directory)
    cmake_parse_arguments(ARG "" "" "DEPENDS;EXCLUDE;SOURCES" ${ARGN})

    if(ARG_SOURCES)
        set(sources "")
        foreach(name IN LISTS ARG_SOURCES)
            if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${name}.cpp")
                message(FATAL_ERROR "opens100_add_module(${target}): SOURCES names ${directory}/${name}.cpp, which does not exist")
            endif()
            list(APPEND sources "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${name}.cpp")
        endforeach()
    else()
        file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.cpp")
    endif()
    foreach(name IN LISTS ARG_EXCLUDE)
        if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${name}.cpp")
            message(FATAL_ERROR "opens100_add_module(${target}): EXCLUDE names ${directory}/${name}.cpp, which does not exist")
        endif()
        list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/${name}.cpp")
    endforeach()
    file(GLOB headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.h")
    list(FILTER headers EXCLUDE REGEX "/(stdafx|targetver|resource)\\.h$")

    add_library(${target} STATIC ${sources})
    add_library(opens100::${target} ALIAS ${target})
    set_target_properties(${target} PROPERTIES
        OUTPUT_NAME "opens100_${target}"
        DEBUG_POSTFIX "d")

    target_include_directories(${target}
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
            "$<INSTALL_INTERFACE:${OPENS100_INSTALL_INCLUDEDIR}>"
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/${directory}")
    target_link_libraries(${target} PUBLIC ${ARG_DEPENDS})

    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8 /W4 /permissive-)
        target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS NOMINMAX)
        if(OPENS100_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra)
        if(OPENS100_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()

    install(TARGETS ${target} EXPORT opens100Targets
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
    if(NOT ARG_SOURCES)
        install(FILES ${headers} DESTINATION "${OPENS100_INSTALL_INCLUDEDIR}/${directory}")
    endif()
endfunction()

# Dependency order: latlon <- geometry, featurecatalog <- portrayalcatalogue; sdb stands alone.
# The portrayal catalogue here is the catalogue data. Its symbol, line style, area fill and colour
# profile files are read by a Portrayal::ResourceStore the application supplies; the Direct2D one
# (S100Engine) stays in the Visual Studio solution.
opens100_add_module(latlon LatLonUtility DEPENDS GeographicLib::GeographicLib)
if(OPENS100_BUILD_GEOMETRY)
    opens100_add_module(geometry GeoMetryLibrary DEPENDS latlon)
endif()
if(OPENS100_BUILD_FEATURECATALOG)
    opens100_add_module(featurecatalog FeatureCatalog DEPENDS latlon pugixml::pugixml utf8cpp::utf8cpp)
endif()
if(OPENS100_BUILD_PORTRAYALCATALOGUE)
    opens100_add_module(portrayalcatalogue PortrayalCatalogue DEPENDS latlon geometry featurecatalog pugixml::pugixml)
endif()
if(OPENS100_BUILD_SYSTEMDATABASE)
    opens100_add_module(sdb SystemDatabase DEPENDS unofficial::sqlite3::sqlite3 pugixml::pugixml OpenSSL::Crypto ZLIB::ZLIB)
endif()

if(OPENS100_BUILD_COVERAGE)
    # S-100 Part 10c gridded coverages: grid model, HDF5 reader, lookups (Documents/TechDocs/2026-11-11-coverage-backport-plan.md).
    opens100_add_module(coverage Coverage DEPENDS ${OPENS100_HDF5_TARGET})
    if(TARGET hdf5::hdf5-shared)
        target_compile_definitions(coverage PUBLIC H5_BUILT_AS_DYNAMIC_LIB)
    endif()
endif()

if(OPENS100_BUILD_DATASET)
    # The dataset model and its readers: S-101 ISO 8211 cells (S101Cell), GML datasets (S10XGML)
    # and HDF5 datasets (S100H5). GISLibrary without its display, portrayal-run and System
    # Database parts; the list of what is left out is in cmake/GISLibraryDatasetSources.cmake.
    include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/GISLibraryDatasetSources.cmake")
    opens100_add_module(dataset GISLibrary
        DEPENDS latlon geometry featurecatalog portrayalcatalogue
                pugixml::pugixml polyclipping::polyclipping Boost::headers ${OPENS100_HDF5_TARGET}
        EXCLUDE ${OPENS100_GISLIBRARY_NON_DATASET_SOURCES})
    # GISLIBRARY_MODEL_ONLY keeps COM, ATL, GDI+ and Direct2D out of the precompiled header.
    target_compile_definitions(dataset
        PRIVATE GISLIBRARY_MODEL_ONLY
        # spatial_association (a plain struct of the Lua scripting module) is named by the feature
        # model; without this it would be declared __declspec(dllimport) on Windows.
        PUBLIC KRS_LUA_SCRIPT_REFERENCE_STATIC)
    if(TARGET hdf5::hdf5-shared)
        target_compile_definitions(dataset PUBLIC H5_BUILT_AS_DYNAMIC_LIB)
    endif()
    if(MSVC)
        # The same conversion warnings GISLibrary.vcxproj disables for these legacy sources.
        target_compile_options(dataset PRIVATE /wd4244 /wd4267 /wd4245 /wd4389 /wd4251)
    else()
        target_compile_options(dataset PRIVATE -Wno-sign-compare -Wno-unused-parameter)
    endif()
    if(NOT OPENS100_BUILD_LUASCRIPTING)
        install(FILES
            LuaScriptingReference/LuaScriptingReference.h
            LuaScriptingReference/spatial_association.h
            DESTINATION "${OPENS100_INSTALL_INCLUDEDIR}/LuaScriptingReference")
    endif()
endif()

if(OPENS100_BUILD_LUASCRIPTING)
    # The Lua side of S-100 Part 9a portrayal: a Lua session, the standard host-call table and the
    # drawing instruction results. S-100 requires Lua 5.1; LuaJIT implements it. The host
    # functions that answer the rules from a dataset are still in the Visual Studio solution.
    # lua_helpers.cpp is in no project (it includes a header that does not exist).
    opens100_add_module(luascripting LuaScriptingReference
        DEPENDS featurecatalog LuaJIT::LuaJIT
        EXCLUDE lua_helpers)
    target_compile_definitions(luascripting PUBLIC KRS_LUA_SCRIPT_REFERENCE_STATIC)
    if(MSVC)
        # LuaScriptingReference.vcxproj builds at /W3; these are the /W4 warnings its sources raise.
        target_compile_options(luascripting PRIVATE /wd4100 /wd4244 /wd4456 /wd4505 /wd4706)
    else()
        target_compile_options(luascripting PRIVATE -Wno-unused-parameter -Wno-unused-function -Wno-shadow)
    endif()
endif()

if(OPENS100_BUILD_PORTRAYAL)
    # What is to be drawn: the portrayal rules of a product run over a dataset and leave drawing
    # instructions (S-100 Part 9) in the dataset's PCOutputSchemaManager. Nothing here draws; a
    # renderer visits the instructions (SENC_InstructionVisitor). The sources are the part of
    # GISLibrary listed in cmake/GISLibraryDatasetSources.cmake.
    opens100_add_module(portrayal GISLibrary
        DEPENDS dataset luascripting LibXslt::LibXslt LibXml2::LibXml2
        SOURCES ${OPENS100_GISLIBRARY_PORTRAYAL_SOURCES})
    target_compile_definitions(portrayal PRIVATE GISLIBRARY_MODEL_ONLY)
    if(MSVC)
        # As for dataset; C4505: the Lua host keeps two unused static helpers.
        target_compile_options(portrayal PRIVATE /wd4244 /wd4267 /wd4245 /wd4389 /wd4251 /wd4505)
    else()
        target_compile_options(portrayal PRIVATE -Wno-sign-compare -Wno-unused-parameter -Wno-unused-function)
    endif()
endif()

if(OPENS100_BUILD_RENDER_SKIA)
    # A backend of the renderer: IRenderer on a Skia canvas, and a picture in memory with its PNG
    # file for drawing with no window (Documents/TechDocs/2026-11-19-r6-skia-backend-design.md).
    # Skia is public: the canvas is the application's.
    opens100_add_module(render-skia RenderSkia DEPENDS portrayal unofficial::skia::skia)
endif()

# --- package ------------------------------------------------------------------------------
set(OPENS100_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/opens100")

install(EXPORT opens100Targets
    NAMESPACE opens100::
    DESTINATION "${OPENS100_INSTALL_CMAKEDIR}")

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/opens100Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/opens100Config.cmake"
    INSTALL_DESTINATION "${OPENS100_INSTALL_CMAKEDIR}")
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/opens100ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)   # 0.x: every minor may break the API

if(OPENS100_BUILD_LUASCRIPTING)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FindLuaJIT.cmake" DESTINATION "${OPENS100_INSTALL_CMAKEDIR}")
endif()
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/opens100Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/opens100ConfigVersion.cmake"
    DESTINATION "${OPENS100_INSTALL_CMAKEDIR}")
