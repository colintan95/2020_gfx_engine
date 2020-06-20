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

  virtual void SubmitCommand(const CommandVariant& command_variant) = 0;

  virtual bool BeginRecording() = 0;
  virtual bool EndRecording() = 0;
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

public:
  static BuilderType BeginBuild(GALPlatform* gal_platform) {
    return BuilderType(gal_platform);
  }

  void Destroy() {
    if (IsValid()) { impl_.Destroy(); }
  }

  template<typename T>
  void SubmitCommand(const T& command_variant) { impl_.SubmitCommand(command_variant); }

  bool BeginRecording() { return impl_.BeginRecording(); }
  bool EndRecording() { return impl_.EndRecording(); }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

private:
  ImplType impl_;
};

} // namespace

#endif // GAL_OBJECT_IMPL_GAL_COMMAND_BUFFER_IMPL_H_