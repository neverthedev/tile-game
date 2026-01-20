#include "camera_component.h"

#include "../game_camera.h"
#include "../graphics/input_system.h"
#include "../graphics/collision_system.h"
#include "raymath.h"

CameraInputComponent::CameraInputComponent(): InputComponent() {}

void CameraInputComponent::HandleInput(GameObject &cam, InputSystem& input, CollisionSystem& collision) {
  GameCamera* camera = dynamic_cast<GameCamera*>(&cam);

  if (!camera) throw GameError("Incorrect object type provided!");

  if (input.IsKeyDown(KEY_RIGHT) || input.IsKeyDown(KEY_D))
    camera->target.x += camera->MOVE_SPEED;
  if (input.IsKeyDown(KEY_LEFT) || input.IsKeyDown(KEY_A))
    camera->target.x -= camera->MOVE_SPEED;
  if (input.IsKeyDown(KEY_DOWN) || input.IsKeyDown(KEY_S))
    camera->target.y += camera->MOVE_SPEED;
  if (input.IsKeyDown(KEY_UP) || input.IsKeyDown(KEY_W))
    camera->target.y -= camera->MOVE_SPEED;

  Position2D mousePos = input.GetMousePosition();
  Vector2 mouse = Vector2{mousePos.x, mousePos.y};
  float wheel = input.GetMouseWheelMove();

  if (wheel != 0.0f) {
    // Convert camera to raylib Camera2D for GetScreenToWorld2D
    Camera2D raylibCam = {
      Vector2{camera->offset.x, camera->offset.y},
      Vector2{camera->target.x, camera->target.y},
      camera->rotation,
      camera->zoom
    };

    Vector2 worldBefore = GetScreenToWorld2D(mouse, raylibCam);

    camera->zoom += wheel * camera->ZOOM_SPEED;
    camera->zoom = Clamp(camera->zoom, camera->ZOOM_MIN, camera->ZOOM_MAX);

    raylibCam.zoom = camera->zoom;
    Vector2 worldAfter = GetScreenToWorld2D(mouse, raylibCam);

    camera->target.x += (worldBefore.x - worldAfter.x);
    camera->target.y += (worldBefore.y - worldAfter.y);
  }
}

CameraInputComponent::~CameraInputComponent() {}
