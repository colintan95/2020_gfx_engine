#ifndef APPLICATION_H_
#define APPLICATION_H_

// TODO(colintan): Remove these external lib references once I've encapsulated them in the modules
#include <GL/glew.h>

#include <memory>
#include "resource/model_loader.h"
#include "window/window.h"

class Application {
public:
  Application();
  ~Application();

  void RunLoop();

private:
  std::unique_ptr<window::Window> window_;

  // TODO(colintan): Remove
  GLuint program_;
  GLuint texture_;
  GLuint vao_;
  GLuint pos_vbo_;
  GLuint texcoord_vbo_;
  std::shared_ptr<resource::Model> model_;
};

#endif // APPLICATION_H_