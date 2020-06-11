#include "../command_buffer.h"

#include <GL/glew.h>

#include "../commands.h"
#include "../objects.h"
#include "../gal.h"
#include "id_converter.h"

namespace gal {

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  for (const GALCommandBuffer::Entry& entry : cmd_buf.entries_) {

    if (entry.IsType<command::SetViewport>()) {
      auto cmd = entry.AsType<command::SetViewport>();
      glViewport(cmd.x, cmd.y, cmd.width, cmd.height);
    
    } else if (entry.IsType<command::ClearScreen>()) {
      auto cmd = entry.AsType<command::ClearScreen>();

      glClearColor(cmd.color.r, cmd.color.g, cmd.color.b, cmd.color.a);
      if (cmd.clear_color) {
        glClear(GL_COLOR_BUFFER_BIT);
      }
      if (cmd.clear_depth) {
        glClear(GL_DEPTH_BUFFER_BIT);
      }

    } else if (entry.IsType<command::SetPipeline>()) {
      auto cmd = entry.AsType<command::SetPipeline>();

      if (std::optional<GLuint> gl_id_opt = opengl::ConvertGALId(cmd.pipeline.GetGALId())) {
        glUseProgram(*gl_id_opt);
      }

    } else if (entry.IsType<command::SetVertexDesc>()) {
      auto cmd = entry.AsType<command::SetVertexDesc>();

      if (std::optional<GLuint> gl_id_opt = opengl::ConvertGALId(cmd.vert_desc.GetGALId())) {
        GLuint gl_id = *gl_id_opt;
        glBindVertexArray(gl_id);

        // TODO(colintan): Support multiple entries
          const GALVertexDesc::Entry& entry = cmd.vert_desc.Index(0);
        if (std::optional<GLuint> gl_vbo_id_opt = opengl::ConvertGALId(entry.buffer.GetGALId())) {
          glBindBuffer(GL_ARRAY_BUFFER, *gl_vbo_id_opt);
          glVertexAttribPointer(entry.index, entry.size, GL_FLOAT, GL_FALSE, 
                                entry.size * sizeof(float), nullptr);
          glEnableVertexAttribArray(entry.index);
        }
      }
    } else if (entry.IsType<command::SetVertexBuffer>()) {
      auto cmd = entry.AsType<command::SetVertexBuffer>();
      
      if (std::optional<GLuint> gl_id_opt = opengl::ConvertGALId(cmd.buffer.GetGALId())) {
        glBindBuffer(GL_ARRAY_BUFFER, *gl_id_opt);
      }

    } else if (entry.IsType<command::DrawTriangles>()) {
      auto cmd = entry.AsType<command::DrawTriangles>();

      glDrawArrays(GL_TRIANGLES, 0, cmd.num_triangles * 3);
    }
  }
}

} // namespace