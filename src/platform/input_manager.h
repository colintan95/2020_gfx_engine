#ifndef INPUT_INPUT_MANAGER_H_
#define INPUT_INPUT_MANAGER_H_

#include <memory>

namespace platform {

namespace internal {
class InputSource;
} // namespace

enum class Keyboard {
  KeyW
};

class InputManager {
public:
  InputManager();
  ~InputManager();

  bool Initialize();
  void Cleanup();

  void Tick();

private:
  std::unique_ptr<internal::InputSource> input_source_;
};

} // namespace

#endif // INPUT_INPUT_MANAGER_H_