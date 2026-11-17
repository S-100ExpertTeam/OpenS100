# Minimal external consumer of the opens100 package.
#
# It is deliberately a separate project: it finds opens100 through vcpkg (overlay port in
# ../../ports), exactly the way the KHOA viewer or any other application does.
#
#   cmake -S examples/consumer -B build/consumer -G Ninja ^
#         -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
#   cmake --build build/consumer
#   build\consumer\opens100_consumer.exe ProgramData\FC\S-101_FC.xml ^
#         ProgramData\PC\S101_Portrayal\portrayal_catalogue.xml <an S-101 cell>.000
cmake_minimum_required(VERSION 3.24)
project(opens100_consumer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(opens100 CONFIG REQUIRED)

add_executable(opens100_consumer main.cpp)
target_link_libraries(opens100_consumer PRIVATE opens100::featurecatalog opens100::geometry opens100::portrayalcatalogue)
# The dataset readers are a port feature ("dataset"); use them when the package has them.
if(TARGET opens100::dataset)
    target_link_libraries(opens100_consumer PRIVATE opens100::dataset)
    target_compile_definitions(opens100_consumer PRIVATE OPENS100_CONSUMER_HAS_DATASET)
endif()
if(MSVC)
    target_compile_options(opens100_consumer PRIVATE /utf-8 /W4)
endif()
