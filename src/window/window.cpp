#include "window/window.h"

#include "window/window_internal.h"

namespace window {

WindowRef::WindowRef(WindowId window_id, WindowInternal* impl) { 
  window_id_ = window_id; 
  impl_ = impl; 
}

} // namespace