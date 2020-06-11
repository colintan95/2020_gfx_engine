#include "command_buffer.h"

#include "commands.h"
#include "internal/interface.h"

namespace gal {

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  internal::ExecuteCommandBuffer(cmd_buf);
}

} // namespace