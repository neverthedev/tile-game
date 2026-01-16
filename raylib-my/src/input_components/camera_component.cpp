#include "camera_component.h"

#include "../game_camera.h"

CameraInputComponent::CameraInputComponent(): InputComponent() {}

void CameraInputComponent::HandleInput(GameObject &cam) {
  GameCamera* camera = dynamic_cast<GameCamera*>(&cam);

  if (!camera) throw GameError("Incorrect object type provided!");

  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    camera->target.x += camera->MOVE_SPEED;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    camera->target.x -= camera->MOVE_SPEED;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    camera->target.y += camera->MOVE_SPEED;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    camera->target.y -= camera->MOVE_SPEED;

  Vector2 mouse = GetMousePosition();
  float wheel = GetMouseWheelMove();

  if (wheel != 0.0f) {
    Vector2 worldBefore = GetScreenToWorld2D(mouse, *camera);

    camera->zoom += wheel * camera->ZOOM_SPEED;
    camera->zoom = Clamp(camera->zoom, camera->ZOOM_MIN, camera->ZOOM_MAX);
    Vector2 worldAfter = GetScreenToWorld2D(mouse, *camera);

    camera->target.x += (worldBefore.x - worldAfter.x);
    camera->target.y += (worldBefore.y - worldAfter.y);
  }
}

CameraInputComponent::~CameraInputComponent() {}
