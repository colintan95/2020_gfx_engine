#ifndef RESOURCE_RESOURCE_SYSTEM_H_
#define RESOURCE_RESOURCE_SYSTEM_H_

#include <memory>
#include <string>
#include "resource/image_loader.h"
#include "resource/model_loader.h"
#include "resource/resource.h"

namespace resource {

class ResourceManager;

class ResourceSystem {
public:
  ResourceSystem();
  ~ResourceSystem();

  bool Initialize();
  void Cleanup();

  Handle<Model> LoadModel(const std::string& file_path);

  Handle<Image> LoadImage(const std::string& file_path);

private:
  std::unique_ptr<ResourceManager> default_manager_;

  ModelLoader model_loader_;
  ImageLoader image_loader_;
};

} // namespace

#endif // RESOURCE_RESOURCE_SYSTEM_H_