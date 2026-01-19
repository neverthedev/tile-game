#pragma once

#include "graphics.h"
#include "common/game_object.h"
#include "common/position_2d.h"
#include "common/grph_camera.h"

class GameCamera: public GameObject {
public:
  const float ZOOM_SPEED = 0.1f;
  const float MOVE_SPEED = 3.0f;
  const float ZOOM_MIN = 0.5f;
  const float ZOOM_MAX = 3.0f;

  Position2D offset;      // Camera offset (displacement from target)
  Position2D target;      // Camera target (rotation and zoom origin)
  float rotation;         // Camera rotation in degrees
  float zoom;             // Camera zoom (scaling), should be 1.0f by default

  GameCamera(std::unique_ptr<InputComponent> inp, std::unique_ptr<GraphicsComponent> grph, std::unique_ptr<UpdateComponent> upd);
  void UpdateFromGrphCamera(const GrphCamera& grphCamera);
  ~GameCamera();
};
