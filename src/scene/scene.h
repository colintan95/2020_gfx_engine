#ifndef SCENE_SCENE_H_
#define SCENE_SCENE_H_

namespace render {
class Renderer;
} // namespace

namespace scene {

class Scene {
public:
  bool Initialize(render::Renderer* renderer);
  void Cleanup();

  void Tick();

private:
  render::Renderer* renderer_;
};

} // namespace

#endif // SCENE_SCENE_H_