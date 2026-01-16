#include "camera_component.h"

#include "../graphics.h"
#include "../game_camera.h"
#include "../common/game_error.h"

CameraGraphicsComponent::CameraGraphicsComponent(): GraphicsComponent() {}

void CameraGraphicsComponent::Render(GameObject& cam, Graphics& grph) {
  GameCamera *camera = dynamic_cast<GameCamera *>(&cam);
  if (!camera) throw GameError("Incorrect object type provided!");

  grph.camera.offset = camera->offset;
  grph.camera.target = camera->target;
  grph.camera.rotation = camera->rotation;
  grph.camera.zoom = camera->zoom;
}

CameraGraphicsComponent::~CameraGraphicsComponent() {}
