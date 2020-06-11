#ifndef GAL_COMMAND_BUFFER_H_
#define GAL_COMMAND_BUFFER_H_

#include <variant>
#include <vector>
#include "commands.h"

namespace gal {

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