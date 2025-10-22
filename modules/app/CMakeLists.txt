add_executable(app src/main.cpp src/graphics/d3d.cpp src/scene/scene.cpp src/app/app.cpp)

target_link_libraries(app spdlog d3d11 D3DCompiler core)

target_compile_features(app PRIVATE cxx_std_20)
