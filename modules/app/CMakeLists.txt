add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/graphics/d3d.cpp
  src/scene/scene.cpp
  src/app/app.cpp
  src/graphics/mesh/base.cpp
  src/graphics/mesh/triangle.cpp
  src/graphics/mesh/sphere.cpp
  src/graphics/camera/camera.cpp
  src/graphics/light/point.h)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR}
                                        ${imgui_SOURCE_DIR}/backends)
target_link_libraries(${PROJECT_NAME} spdlog imgui d3d11 D3DCompiler core)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# Path to fxc.exe
set(FXC_PATH
    "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x64/fxc.exe")

# Help from copilot here
file(GLOB VERTEX_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex/*.hlsl")
file(GLOB PIXEL_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/shaders/pixel/*.hlsl")

set(HLSL_SHADER_FILES ${VERTEX_SHADERS} ${PIXEL_SHADERS})

foreach(FILE ${VERTEX_SHADERS})
  set_source_files_properties(${FILE} PROPERTIES ShaderType "vs" ShaderModel
                                                                 "5_0")
endforeach()

foreach(FILE ${PIXEL_SHADERS})
  set_source_files_properties(${FILE} PROPERTIES ShaderType "ps" ShaderModel
                                                                 "5_0")
endforeach()

# Compile shaders
foreach(FILE ${HLSL_SHADER_FILES})
  get_filename_component(FILE_WE ${FILE} NAME_WE)
  get_source_file_property(shadertype ${FILE} ShaderType)
  get_source_file_property(shadermodel ${FILE} ShaderModel)

  # Set output directory based on shader type
  if(shadertype STREQUAL "vs")
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders/vertex")
  elseif(shadertype STREQUAL "ps")
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders/pixel")
  endif()

  file(MAKE_DIRECTORY ${OUTPUT_DIR})

  set(CSO_FILE "${OUTPUT_DIR}/${FILE_WE}.cso")
  set(PDB_FILE "${OUTPUT_DIR}/${FILE_WE}.pdb")

  list(APPEND CSO_SHADER_FILES ${CSO_FILE})

  # Zi and Fo flags for debugging
  add_custom_command(
    TARGET ${PROJECT_NAME}
    PRE_LINK
    COMMAND
      ${FXC_PATH} /nologo /Emain /T${shadertype}_${shadermodel}
      $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CSO_FILE} /Fd ${PDB_FILE} ${FILE}
    COMMENT "Compiling HLSL shader: ${FILE}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM)
endforeach()

# Copy compiled shaders after build
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CSO_SHADER_FILES}
          $<TARGET_FILE_DIR:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS)
