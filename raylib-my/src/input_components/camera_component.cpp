#include "camera_component.h"

#include "../game_camera.h"
#include "../graphics/input_system.h"
#include "../graphics/collision_system.h"
#include "../common/keyboard_2d.h"

CameraInputComponent::CameraInputComponent(): InputComponent() {}

void CameraInputComponent::HandleInput(GameObject &cam, InputSystem& input, CollisionSystem& collision) {
  GameCamera* camera = dynamic_cast<GameCamera*>(&cam);

  if (!camera) throw GameError("Incorrect object type provided!");

  if (input.IsKeyDown(Keyboard2D::KEY_RIGHT) || input.IsKeyDown(Keyboard2D::KEY_D))
    camera->target.x += camera->MOVE_SPEED;
  if (input.IsKeyDown(Keyboard2D::KEY_LEFT) || input.IsKeyDown(Keyboard2D::KEY_A))
    camera->target.x -= camera->MOVE_SPEED;
  if (input.IsKeyDown(Keyboard2D::KEY_DOWN) || input.IsKeyDown(Keyboard2D::KEY_S))
    camera->target.y += camera->MOVE_SPEED;
  if (input.IsKeyDown(Keyboard2D::KEY_UP) || input.IsKeyDown(Keyboard2D::KEY_W))
    camera->target.y -= camera->MOVE_SPEED;

  if (float wheel = input.GetMouseWheelMove(); wheel != 0.0f) {
    camera->pendingWheel = wheel;
  }
}

CameraInputComponent::~CameraInputComponent() {}
