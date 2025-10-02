set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 4.0.2)
project(GameLoopMessageHandling LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -gdwarf-4 -O0")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-Wl,-debug")
endif()

file(
  GLOB
  SOURCES
  src/graphics/*.cpp
  src/app/*.cpp
  src/vertices/*.cpp
  src/camera/*.cpp
  src/game/*.cpp
  src/input/*.cpp
  src/utils/*.cpp
  src/*.cpp)
add_executable(app WIN32 ${SOURCES})
target_link_libraries(app d3d11 D3DCompiler)
