add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/state/state.cpp
  src/texturemanager/texturemanager.cpp
  src/shadermanager/shadermanager.cpp
  src/scene/scene.cpp
  src/postprocessors/renderstage.cpp
  src/postprocessors/crt.cpp
  src/scene/debugScene.cpp
  src/input/input.cpp
  src/app/app.cpp
  src/buffer/matrix.cpp
  src/buffer/ambientLight.cpp
  src/buffer/pointLights.cpp
  src/buffer/spotLights.cpp
  src/buffer/directionalLights.cpp
  src/buffer/waveData.cpp
  src/buffer/crt.cpp
  src/buffer/camera.cpp)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}
                                        ${imgui_SOURCE_DIR}/backends)

target_link_libraries(
  ${PROJECT_NAME}
  spdlog
  imgui
  d3d11
  D3DCompiler
  core_window
  core_input
  core_utils
  core_graphics
  shared_shader)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CSO_SHADER_FILES}
          $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS)
