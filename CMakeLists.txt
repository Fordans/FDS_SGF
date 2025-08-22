cmake_minimum_required(VERSION 3.10.0)
project(FDS_SGF VERSION 0.1.0 LANGUAGES C CXX)

# compile with utf-8 for MSVC spdlog
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(src)
include_directories(include)
link_directories(lib)

add_executable(
    FDS_SGF
src/main.cpp
src/core/Game.cpp
src/core/Scene.h
src/game/Object.h
src/game/DemoScene.cpp
)

target_link_options(
    FDS_SGF
PRIVATE
$<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
$<$<CONFIG:Release>:/SUBSYSTEM:WINDOWS>
)

target_link_libraries(
    FDS_SGF
SDL3
SDL3_image
SDL3_mixer
SDL3_ttf
)