#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <memory>
#include "gal/gal.h"
#include "resource/model_loader.h"
#include "window/window.h"

class Application {
public:
  Application();
  ~Application();

  void RunLoop();

private:
  std::unique_ptr<window::Window> window_;
  gal::GALCommandBuffer command_buffer_;
};

#endif // APPLICATION_H_