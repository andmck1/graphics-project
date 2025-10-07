add_executable(app src/main.cpp)

target_link_libraries(app PRIVATE spdlog)
target_link_libraries(app PRIVATE core)
target_compile_features(app PRIVATE cxx_std_20)
