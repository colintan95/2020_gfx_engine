#include "resource/resource_system.h"

#include <memory>
#include "resource/resource_manager.h"
#include "resource/model_loader.h"

namespace resource {

ResourceSystem::ResourceSystem() {

}

ResourceSystem::~ResourceSystem() {
  
}

bool ResourceSystem::Initialize() {
  default_manager_ = std::make_unique<ResourceManager>();
  return true;
}

void ResourceSystem::Cleanup() {
  default_manager_.release();
}

Handle<Model> ResourceSystem::LoadModel(const std::string& file_path) {
  Handle<Model> resource_handle = default_manager_->CreateResource<Model>();
  if (!model_loader_.LoadModel(file_path, resource_handle.Get())) {
    return Handle<Model>();
  }
  return resource_handle;
}

Handle<Image> ResourceSystem::LoadImage(const std::string& file_path) {
  Handle<Image> resource_handle = default_manager_->CreateResource<Image>();
  if (!image_loader_.LoadImage(file_path, resource_handle.Get())) {
    return Handle<Image>();
  }
  return resource_handle;
}

} // namespace resource