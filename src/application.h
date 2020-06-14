#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <memory>
#include "gal/gal.h"
#include "resource/model_loader.h"
#include "window/window.h"

class Application {
public:
  bool Initialize(window::Window* window_);
  void Cleanup();

  void Tick();

private:
  window::Window* window_;
  gal::GALPlatform gal_platform_;
  gal::GALCommandBuffer command_buffer_;
};

#endif // APPLICATION_H_