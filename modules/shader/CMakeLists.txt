# :TODO - make this not hardcoded
set(FXC_PATH
    "C:/Program Files (x86)/Windows Kits/10/bin/10.0.22621.0/x86/fxc.exe")
# set(FXC_PATH "C:/Program Files (x86)/Windows
# Kits/10/bin/10.0.22621.0/x86/fxc.exe")

add_library(shared_shader STATIC src/shared_shader/sharedbuffertypes.h
                                 src/shared_shader/sharedbuffertypes.cpp)
target_link_libraries(shared_shader d3d11 D3DCompiler)
target_include_directories(shared_shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Help from copilot here and
# https://stackoverflow.com/questions/71299716/how-to-compile-hlsl-shaders-during-build-with-cmake
set(VERTEX_SHADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/vertex/standard.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/vertex/heightMap.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/vertex/wave.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/vertex/depth.hlsl")
set(PIXEL_SHADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/pixel/lighting.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/pixel/textureOnly.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/pixel/crt.hlsl"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/hlsl/pixel/depth.hlsl")
set(HLSL_SHADER_FILES ${VERTEX_SHADERS} ${PIXEL_SHADERS})
set(CMAKE_HLSL_OUTPUT_DIR ${CMAKE_BINARY_DIR}/hlsl)

foreach(FILE ${VERTEX_SHADERS})
  set_source_files_properties(${FILE} PROPERTIES ShaderType "vs" ShaderModel
                                                                 "5_0")
endforeach()

foreach(FILE ${PIXEL_SHADERS})
  set_source_files_properties(${FILE} PROPERTIES ShaderType "ps" ShaderModel
                                                                 "5_0")
endforeach()

foreach(FILE ${HLSL_SHADER_FILES})
  get_filename_component(FILE_WE ${FILE} NAME_WE)
  get_source_file_property(shadertype ${FILE} ShaderType)
  get_source_file_property(shadermodel ${FILE} ShaderModel)

  if(shadertype STREQUAL "vs")
    set(OUTPUT_DIR "${CMAKE_HLSL_OUTPUT_DIR}/vertex")
  elseif(shadertype STREQUAL "ps")
    set(OUTPUT_DIR "${CMAKE_HLSL_OUTPUT_DIR}/pixel")
  endif()

  file(MAKE_DIRECTORY ${OUTPUT_DIR})
  set(CSO_FILE "${OUTPUT_DIR}/${FILE_WE}.cso")
  set(PDB_FILE "${OUTPUT_DIR}/${FILE_WE}.pdb")
  list(APPEND CSO_SHADER_FILES ${CSO_FILE})

  set(FXC_ARGS
      /nologo
      /Emain
      /T${shadertype}_${shadermodel}
      /I
      ${CMAKE_CURRENT_SOURCE_DIR}/src/shared_shader/
      $<IF:$<CONFIG:DEBUG>,/Od,/O1>
      /Zi
      /Fo
      ${CSO_FILE}
      /Fd
      ${PDB_FILE}
      ${FILE})

  # Zi and Fo flags for debugging
  add_custom_command(
    OUTPUT ${CSO_FILE}
    COMMAND ${FXC_PATH} ${FXC_ARGS}
    DEPENDS ${FILE}
    COMMENT "Compiling HLSL shader: ${FILE}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    VERBATIM)
  add_custom_target(compile_shaders_${FILE_WE}_${shadertype} ALL DEPENDS ${CSO_SHADER_FILES})
endforeach()

set(CSO_SHADER_FILES
    ${CSO_SHADER_FILES}
    PARENT_SCOPE)
