#include "camera_component.h"

#include <algorithm>

#include "../graphics/render_system.h"
#include "../game_camera.h"
#include "../common/game_error.h"
#include "../common/grph_camera.h"

CameraGraphicsComponent::CameraGraphicsComponent(): GraphicsComponent() {}

void CameraGraphicsComponent::Render(GameObject& cam, RenderSystem& renderer) {
  GameCamera *camera = dynamic_cast<GameCamera *>(&cam);
  if (!camera) throw GameError("Incorrect object type provided!");

  if (camera->pendingWheel != 0.0f) {
    GrphCamera beforeCam;
    beforeCam.offset = camera->offset;
    beforeCam.target = camera->target;
    beforeCam.rotation = camera->rotation;
    beforeCam.zoom = camera->zoom;

    Position2D worldBefore = renderer.ScreenToWorldWithCamera(beforeCam);

    camera->zoom += camera->pendingWheel * camera->ZOOM_SPEED;
    camera->zoom = std::clamp(camera->zoom, camera->ZOOM_MIN, camera->ZOOM_MAX);

    GrphCamera afterCam = beforeCam;
    afterCam.zoom = camera->zoom;
    Position2D worldAfter = renderer.ScreenToWorldWithCamera(afterCam);

    camera->target.x += (worldBefore.x - worldAfter.x);
    camera->target.y += (worldBefore.y - worldAfter.y);

    camera->pendingWheel = 0.0f;
  }

  GrphCamera grphCam;
  grphCam.offset = camera->offset;
  grphCam.target = camera->target;
  grphCam.rotation = camera->rotation;
  grphCam.zoom = camera->zoom;

  renderer.UpdateGrphCamera(grphCam);
}

CameraGraphicsComponent::~CameraGraphicsComponent() {}
