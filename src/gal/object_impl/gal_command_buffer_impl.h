#ifndef GAL_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_H_

#include <exception>
#include "gal/commands.h"
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

class IGALCommandBufferImpl {
public:
  virtual ~IGALCommandBufferImpl() {}

  virtual void Destroy() = 0;

  virtual void AddCommand(const command::CommandUnion& command) = 0;
};

template<typename ImplType, typename BuilderType>
class GALCommandBufferBase : public GALObjectBase {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize GALCommandBuffer.";
    }
  };

  struct Entry {
    command::CommandUnion cmd;

    template<typename T>
    bool IsType() const { return std::holds_alternative<T>(cmd); }

    template<typename T>
    const T& AsType() const { return std::get<T>(cmd); }
  };

public:
  static BuilderType BeginBuild(GALPlatform* gal_platform) {
    return BuilderType(gal_platform);
  }

  void Destroy() {
    if (IsValid()) { impl_.Destroy(); }
  }

  template<typename T>
  void AddCommand(const T& command) { impl_.AddCommand(command); }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  ImplType impl_;
};

} // namespace

#endif // GAL_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_H_