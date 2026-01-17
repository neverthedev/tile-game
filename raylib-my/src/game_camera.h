#pragma once

#include "raylib.h"

#include "graphics.h"
#include "common/game_object.h"

class GameCamera: public GameObject {
public:
  const float ZOOM_SPEED = 0.1f;
  const float MOVE_SPEED = 3.0f;
  const float ZOOM_MIN = 0.5f;
  const float ZOOM_MAX = 3.0f;

  Vector2 offset;         // Camera offset (displacement from target)
  Vector2 target;         // Camera target (rotation and zoom origin)
  float rotation;         // Camera rotation in degrees
  float zoom;             // Camera zoom (scaling), should be 1.0f by default

  GameCamera(std::unique_ptr<InputComponent> inp, std::unique_ptr<GraphicsComponent> grph);
  void Update() override;
  void UpdateFromCamera2D(Camera2D);
  ~GameCamera();

  operator const Camera2D() const;

private:
};
