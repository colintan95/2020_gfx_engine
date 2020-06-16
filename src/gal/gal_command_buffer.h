#ifndef GAL_GAL_COMMAND_BUFFER_
#define GAL_GAL_COMMAND_BUFFER_

#include "commands.h"

namespace gal {

// TODO(colintan): Consider adding a Start() and End() function to be called before and after
// adding commands - helps safeguard against the user accidentally adding more commands to a
// command buffer that they have already considered finished
class GALCommandBuffer {
  friend class GALPlatform;
  
public:
  // TODO(colintan): How to do this better
  void SetPlatform(GALPlatform* platform) {
    platform_ = platform;
  }

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

private:
  void Execute() const;

private:
  GALPlatform* platform_;
  std::vector<Entry> entries_;
};

} // namespace

#endif // GAL_GAL_COMMAND_BUFFER_