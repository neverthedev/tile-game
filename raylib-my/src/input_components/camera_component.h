#pragma once

#include "component.h"
#include "../common/game_error.h"
#include "../common/game_object.h"

// Forward declaration
class GameCamera;

class CameraInputComponent: public InputComponent {
public:
  CameraInputComponent();
  virtual void HandleInput(GameObject&, InputSystem&, CollisionSystem&) override;
  ~CameraInputComponent() override;
};
