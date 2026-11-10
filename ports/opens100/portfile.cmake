# opens100 - overlay port.
#
# Two ways to get the sources:
#   * The port directory sits inside an OpenS100 checkout (<repo>/ports/opens100): build that
#     checkout. This is the development setup; a consumer points "overlay-ports" at <repo>/ports.
#     vcpkg does not see source edits, so run `vcpkg remove opens100` (or bump "port-version")
#     to rebuild after changing the library.
#   * The port was copied elsewhere (a registry): fetch the pinned commit from GitHub.
get_filename_component(_opens100_checkout "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

if(EXISTS "${_opens100_checkout}/CMakeLists.txt" AND EXISTS "${_opens100_checkout}/FeatureCatalog")
    set(SOURCE_PATH "${_opens100_checkout}")
    message(STATUS "opens100: building the checkout at ${SOURCE_PATH}")
else()
    vcpkg_from_git(
        OUT_SOURCE_PATH SOURCE_PATH
        URL https://github.com/S-100ExpertTeam/OpenS100.git
        REF 0000000000000000000000000000000000000000   # set to the released commit when publishing
    )
endif()

vcpkg_check_linkage(ONLY_STATIC_LIBRARY)

vcpkg_check_features(OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        feature-catalogue OPENS100_BUILD_FEATURECATALOG
        geometry          OPENS100_BUILD_GEOMETRY
        system-database   OPENS100_BUILD_SYSTEMDATABASE
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS ${FEATURE_OPTIONS}
)
vcpkg_cmake_install()
vcpkg_cmake_config_fixup(PACKAGE_NAME opens100 CONFIG_PATH share/opens100)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include" "${CURRENT_PACKAGES_DIR}/debug/share")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE.txt")
file(INSTALL "${CMAKE_CURRENT_LIST_DIR}/usage" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")
