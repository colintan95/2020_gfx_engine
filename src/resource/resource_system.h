#ifndef RESOURCE_RESOURCE_SYSTEM_H_
#define RESOURCE_RESOURCE_SYSTEM_H_

#include <exception>
#include <memory>
#include <string>
#include "resource/image_loader.h"
#include "resource/model_loader.h"
#include "resource/resource.h"

namespace resource {

class ResourceManager;

class ResourceSystem {
public:
  class InitException : public std::exception {
  public:
    const char* what() const final {
      return "Failed to initialize ResourceSystem.";
    }
  };

public:
  ResourceSystem();
  ~ResourceSystem();

  Handle<Model> LoadModel(const std::string& file_path);

  Handle<Image> LoadImage(const std::string& file_path);

private:
  std::unique_ptr<ResourceManager> default_manager_;

  ModelLoader model_loader_;
  ImageLoader image_loader_;
};

} // namespace

#endif // RESOURCE_RESOURCE_SYSTEM_H_