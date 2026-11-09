# Minimal external consumer of the opens100 package.
#
# It is deliberately a separate project: it finds opens100 through vcpkg (overlay port in
# ../../ports), exactly the way the KHOA viewer or any other application does.
#
#   cmake -S examples/consumer -B build/consumer -G Ninja ^
#         -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
#   cmake --build build/consumer
#   build\consumer\opens100_consumer.exe ProgramData\FC\S-101_FC.xml
cmake_minimum_required(VERSION 3.24)
project(opens100_consumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(opens100 CONFIG REQUIRED)

add_executable(opens100_consumer main.cpp)
target_link_libraries(opens100_consumer PRIVATE opens100::featurecatalog opens100::geometry)
if(MSVC)
    target_compile_options(opens100_consumer PRIVATE /utf-8 /W4)
endif()
