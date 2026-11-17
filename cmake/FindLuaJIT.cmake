# FindLuaJIT - LuaJIT (the Lua 5.1 implementation S-100 Part 9a portrayal runs on).
#
# The LuaJIT build installs no CMake package, only headers, a library and a pkg-config file;
# this module finds the first two. Works with the vcpkg "luajit" port (include/luajit,
# lua51.lib or libluajit-5.1) and with system packages (include/luajit-2.1).
#
# Defines the imported target LuaJIT::LuaJIT and LuaJIT_FOUND.
find_path(LuaJIT_INCLUDE_DIR
    NAMES luajit.h
    PATH_SUFFIXES luajit luajit-2.1 luajit-2.0)
find_library(LuaJIT_LIBRARY
    NAMES luajit-5.1 lua51 luajit)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LuaJIT
    REQUIRED_VARS LuaJIT_LIBRARY LuaJIT_INCLUDE_DIR)

if(LuaJIT_FOUND AND NOT TARGET LuaJIT::LuaJIT)
    add_library(LuaJIT::LuaJIT UNKNOWN IMPORTED)
    set_target_properties(LuaJIT::LuaJIT PROPERTIES
        IMPORTED_LOCATION "${LuaJIT_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LuaJIT_INCLUDE_DIR}")
    if(UNIX)
        # A static LuaJIT needs the dynamic loader and the math library.
        set_property(TARGET LuaJIT::LuaJIT APPEND PROPERTY
            INTERFACE_LINK_LIBRARIES ${CMAKE_DL_LIBS} m)
    endif()
endif()

mark_as_advanced(LuaJIT_INCLUDE_DIR LuaJIT_LIBRARY)
