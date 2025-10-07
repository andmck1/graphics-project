add_library(
    core 
    STATIC 
    src/window/window.cpp src/window/window.h 
    src/graphics/system/d3d.cpp src/graphics/system/d3d.h
    src/graphics/system/camera.cpp src/graphics/system/camera.h
    src/graphics/scene/base.cpp src/graphics/scene/base.h
    src/graphics/scene/default.cpp src/graphics/scene/default.h
)
target_link_libraries(core PRIVATE spdlog)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

