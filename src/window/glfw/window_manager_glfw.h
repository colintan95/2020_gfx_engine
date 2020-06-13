#ifndef WINDOW_GLFW_WINDOW_MANAGER_GLFW_H_
#define WINDOW_GLFW_WINDOW_MANAGER_GLFW_H_

#include "window/window_manager.h"

namespace window {

class WindowManagerImplGLFW : public WindowManagerImpl {
public:
  bool Initialize() final;
  void Cleanup() final;
};

} // namespace

#endif // WINDOW_GLFW_WINDOW_MANAGER_GLFW_H_