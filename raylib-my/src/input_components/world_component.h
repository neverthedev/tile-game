#pragma once

#include "component.h"

// Forward declarations
class GameObject;
class GameWorld;

class WorldInputComponent: public InputComponent {
public:
  WorldInputComponent();
  virtual void HandleInput(GameObject&, InputSystem&, CollisionSystem&) override;
  ~WorldInputComponent() override;
};
