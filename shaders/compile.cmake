set(SHADER_SRC_FILES "${CMAKE_ARGV3}")
set(SHADER_SRC_DIR "${CMAKE_ARGV4}")
set(OUTPUT_DIR "${CMAKE_ARGV5}")
set(COMPILER_PATH "${CMAKE_ARGV6}")

message("${SHADER_SRC_FILES}")

message("cmd /c ${COMPILER_PATH} triangle.vert -o triangle_vert.spv")

execute_process(COMMAND cmd /c "${COMPILER_PATH}" triangle.vert -o triangle_vert.spv
    WORKING_DIRECTORY "${SHADER_SRC_DIR}")

foreach(file ${SHADER_SRC_FILES})
  execute_process(COMMAND "${CMAKE_COMMAND}" -E copy "${SHADER_SRC_DIR}/${file}" 
      "${OUTPUT_DIR}/${file}")
endforeach(file)