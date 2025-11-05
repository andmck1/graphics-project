set(CMAKE_CXX_STANDARD 20)
cmake_minimum_required(VERSION 4.0.2)
project(GraphicsProject LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -gdwarf-4 -O0")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-Wl,-debug")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog
  GIT_TAG v1.15.3
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(spdlog)

include(FetchContent)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.92.4
  GIT_SHALLOW ON
  EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(imgui)

set(DIRECTX_ARCH x86)
include(FetchContent)
FetchContent_Declare(
    directxtk
    GIT_REPOSITORY https://github.com/microsoft/DirectXTK
    GIT_TAG oct2025
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(directxtk)

add_library(
  imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)

add_library(
    directxtk STATIC
    ${DirectXTK_SOURCE_DIR}/Src/WICTextureLoader.cpp
)

add_subdirectory(modules/core)
add_subdirectory(modules/shader)
add_subdirectory(modules/app)
