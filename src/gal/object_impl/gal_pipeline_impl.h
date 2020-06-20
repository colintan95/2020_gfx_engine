#ifndef GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_
#define GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_

#include <cstdint>
#include <exception>
#include <utility>
#include <vector>
#include "gal/gal_shader.h"
#include "gal/object_impl/gal_object.h"

namespace gal {

class GALPlatform;

class IGALPipelineImpl {
public:
  virtual ~IGALPipelineImpl() {}
  
  virtual void Destroy() = 0;
};

template<typename ImplType, typename BuilderType>
class GALPipelineBase : public GALObjectBase {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize GALPipeline";
    }
  };

public:
  class VertexDesc {
  public:
    struct Entry {
      uint8_t index;
      uint8_t size;
      // TODO(colintan): Add more fields
    };

  public:
    void SetAttribute(uint8_t index, uint8_t size) {
      Entry entry;
      entry.index = index;
      entry.size = size;
      entries_.push_back(std::move(entry));
    }

    const std::vector<Entry>& GetEntries() const { return entries_; }

  private:
    // TODO(colintan): This will probably be copied when GALPipeline is copied (e.g. when
    // we copy the GALPipeline into the command buffer). Consider using shared_ptr
    std::vector<Entry> entries_;
  };

public:
  static BuilderType BeginBuild(GALPlatform* gal_platform) {
    return BuilderType(gal_platform);
  }

  void Destroy() {
    if (IsValid()) { impl_.Destroy(); }
  }

  const ImplType& GetImpl() const { return impl_; }
  ImplType& GetImpl() { return impl_; }

public:
 const VertexDesc& GetVertexDesc() const { return vert_desc_; }
  VertexDesc& GetVertexDesc() { return vert_desc_; }

private:
  ImplType impl_;

  VertexDesc vert_desc_;
};

} // namespace

#endif //GAL_OBJECT_IMPL_GAL_PIPELINE_IMPL_H_