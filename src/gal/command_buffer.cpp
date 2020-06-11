#include "command_buffer.h"

#include "commands.h"
#include "internal/interface.h"

namespace gal {

void GALCommandBuffer::AddInternal(command::CommandType type, command::CommandUnion cmd) {
  Entry entry;
  entry.type = type;
  entry.cmd = cmd;
  entries_.push_back(entry);
}

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf) {
  internal::ExecuteCommandBuffer(cmd_buf);
}

} // namespace