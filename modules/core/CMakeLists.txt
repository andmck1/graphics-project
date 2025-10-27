add_library(
    core 
    STATIC 
    src/window/window.cpp src/window/window.h 
    src/input/input.h
)
target_link_libraries(core PRIVATE spdlog imgui)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR}/imgui ${CMAKE_CURRENT_LIST_DIR}/imgui/backends)

