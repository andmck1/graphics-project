add_library(core STATIC src/window/window.cpp src/window/window.h)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

