#ifndef GAL_COMMAND_BUFFER_H_
#define GAL_COMMAND_BUFFER_H_

#include <variant>
#include <vector>
#include "commands.h"

namespace gal {

// TODO(colintan): Consider moving this into the commands.h file and changing 
// command_buffer_gl.cpp to commands_gl.cpp (since this is where the commands are executed)

// TODO(colintan): Consider adding a Start() and End() function to be called before and after
// adding commands - helps safeguard against the user accidentally adding more commands to a
// command buffer that they have already considered finished
class GALCommandBuffer {
public:
  template<typename T>
  void Add(T command) {
    Entry entry;
    entry.cmd = command;
    entries_.push_back(entry);
  }

public:
  struct Entry {
    command::CommandUnion cmd;

    template<typename T>
    bool IsType() const { return std::holds_alternative<T>(cmd); }

    template<typename T>
    const T& AsType() const { return std::get<T>(cmd); }
  };

public:
  std::vector<Entry> entries_;
};

void ExecuteCommandBuffer(const GALCommandBuffer& cmd_buf);

} // namespace

#endif // GAL_COMMAND_BUFFER_H_