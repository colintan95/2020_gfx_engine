#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "gal/gal.h"
#include "resource/model_loader.h"
#include "window/window.h"

class Application {
public:
  bool Initialize(window::WindowRef window_ref);
  void Cleanup();

  void Tick();

private:
  window::WindowRef window_ref_;
  gal::GALCommandBuffer command_buffer_;
};

#endif // APPLICATION_H_