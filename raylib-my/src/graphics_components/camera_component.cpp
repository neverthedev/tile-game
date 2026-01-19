#include "camera_component.h"

#include "../graphics.h"
#include "../game_camera.h"
#include "../common/game_error.h"
#include "../common/grph_camera.h"

CameraGraphicsComponent::CameraGraphicsComponent(): GraphicsComponent() {}

void CameraGraphicsComponent::Render(GameObject& cam, Graphics& grph) {
  GameCamera *camera = dynamic_cast<GameCamera *>(&cam);
  if (!camera) throw GameError("Incorrect object type provided!");

  GrphCamera grphCam;
  grphCam.offset = camera->offset;
  grphCam.target = camera->target;
  grphCam.rotation = camera->rotation;
  grphCam.zoom = camera->zoom;

  grph.UpdateGrphCamera(grphCam);
}

CameraGraphicsComponent::~CameraGraphicsComponent() {}
