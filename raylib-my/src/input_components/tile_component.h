#pragma once

#include "component.h"

// Forward declaration
class WorldTile;

class TileInputComponent: public InputComponent {
public:
  TileInputComponent();
  virtual void HandleInput(GameObject&, InputSystem&, CollisionSystem&) override;
  ~TileInputComponent() override;
};
