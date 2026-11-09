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
    VERSION 0.1.0
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

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

find_package(GeographicLib CONFIG REQUIRED)
if(OPENS100_BUILD_FEATURECATALOG)
    find_package(pugixml CONFIG REQUIRED)
    find_package(utf8cpp CONFIG REQUIRED)
endif()

# Headers are installed as <prefix>/include/opens100/<Module>/<header>, the same relative layout
# as the source tree, so the modules' "../Other/x.h" includes keep working after installation and
# consumers write  #include <FeatureCatalog/FeatureCatalogue.h>.
set(OPENS100_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/opens100")

# opens100_add_module(<target> <directory> [DEPENDS <targets>...])
# One static library per source directory, every *.cpp in it (the .vcxproj lists exactly those).
function(opens100_add_module target directory)
    cmake_parse_arguments(ARG "" "" "DEPENDS" ${ARGN})

    file(GLOB sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.cpp")
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
    install(FILES ${headers} DESTINATION "${OPENS100_INSTALL_INCLUDEDIR}/${directory}")
endfunction()

# Dependency order: latlon <- geometry, featurecatalog. (PortrayalCatalogue still includes the
# Direct2D render module and joins after step 2-3c of the Phase 1 plan.)
opens100_add_module(latlon LatLonUtility DEPENDS GeographicLib::GeographicLib)
if(OPENS100_BUILD_GEOMETRY)
    opens100_add_module(geometry GeoMetryLibrary DEPENDS latlon)
endif()
if(OPENS100_BUILD_FEATURECATALOG)
    opens100_add_module(featurecatalog FeatureCatalog DEPENDS latlon pugixml::pugixml utf8cpp::utf8cpp)
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

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/opens100Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/opens100ConfigVersion.cmake"
    DESTINATION "${OPENS100_INSTALL_CMAKEDIR}")
