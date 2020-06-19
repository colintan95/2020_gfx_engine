set(SHADER_SRC_FILES
    "triangle.frag"
    "triangle.vert"
)

set(SHADER_BUILD_FILES)

foreach(file ${SHADER_SRC_FILES})
  string(REGEX REPLACE "(.*).(frag|vert)" "\\1_\\2" new_name ${file})

  string(CONCAT new_path ${new_name} ".spv")

  add_custom_command(OUTPUT ${new_path}
    COMMAND ${CMAKE_COMMAND} -E touch ${new_path}
    MAIN_DEPENDENCY ${file}
    COMMENT "Creating ${new_path}")

  list(APPEND SHADER_BUILD_FILES ${new_path})

  # add_custom_target(${new_name} ALL
  #     COMMAND echo ${new_name}
  #     DEPENDS ${new_path})
endforeach(file)

add_custom_target(shaders ALL
    COMMAND echo "Add targets for ${SHADER_BUILD_FILES}"
    DEPENDS ${SHADER_BUILD_FILES})

# add_custom_target(shaders ALL
#     COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_SOURCE_DIR}/compile.cmake" "${SHADER_SRC_FILES}"
#         "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}" "${GLSL_COMPILER_PATH}"
#     DEPENDS "${SHADER_SRCE_FILES}"
# )