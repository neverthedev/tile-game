#ifndef CAMERA_GRAPHICS_COMPONENT
#define CAMERA_GRAPHICS_COMPONENT

#include "./component.cpp"
#include "../graphics.cpp"
#include "../game_camera.cpp"
#include "../common/game_error.cpp"

class CameraGraphicsComponent: public GraphicsComponent {
  public:
    CameraGraphicsComponent(): GraphicsComponent() {};
    virtual void Render(GameObject&, Graphics&) override;
    ~CameraGraphicsComponent() override {};

  private:

};

void CameraGraphicsComponent::Render(GameObject& cam, Graphics& grph) {
  GameCamera *camera = dynamic_cast<GameCamera *>(&cam);
  if (!camera) throw GameError("Incorrect object type provided!");

  grph.camera.offset = camera->offset;
  grph.camera.target = camera->target;
  grph.camera.rotation = camera->rotation;
  grph.camera.zoom = camera->zoom;
}

#endif // CAMERA_GRAPHICS_COMPONENT
