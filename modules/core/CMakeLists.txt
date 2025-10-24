add_library(
    core 
    STATIC 
    src/window/window.cpp src/window/window.h 
    src/input/input.h
)
target_link_libraries(core PRIVATE spdlog)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

