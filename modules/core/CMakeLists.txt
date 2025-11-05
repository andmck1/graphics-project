add_library(core_window STATIC src/core_window/window.cpp)
target_link_libraries(core_window PRIVATE spdlog imgui)
target_include_directories(core_window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(core_input STATIC src/core_input/input.cpp)
target_link_libraries(core_input PRIVATE spdlog imgui)
target_include_directories(core_input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(core_utils STATIC src/core_utils/timer.cpp)
target_link_libraries(core_utils PRIVATE spdlog imgui)
target_include_directories(core_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_library(
  core_graphics STATIC
  src/core_graphics/d3d/d3d.cpp
  src/core_graphics/shader/shader.cpp
  src/core_graphics/shader/shadermanager.cpp
  src/core_graphics/texture/texture.cpp
  src/core_graphics/mesh/base.cpp
  src/core_graphics/mesh/triangle.cpp
  src/core_graphics/mesh/sphere.cpp
  src/core_graphics/mesh/quad.cpp
  src/core_graphics/mesh/plane.cpp
  src/core_graphics/mesh/ortho.cpp
  src/core_graphics/camera/camera.cpp
  )
target_link_libraries(core_graphics PRIVATE spdlog imgui)
target_include_directories(core_graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_include_directories(
  imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui
               ${CMAKE_CURRENT_LIST_DIR}/imgui/backends)
