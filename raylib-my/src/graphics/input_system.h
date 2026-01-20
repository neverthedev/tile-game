#pragma once

#include "../common/position_2d.h"

class InputSystem {
public:
  virtual ~InputSystem() = default;

  virtual Position2D GetMousePosition() const = 0;
  virtual bool IsKeyPressed(int key) const = 0;
  virtual bool IsMouseButtonPressed(int button) const = 0;
  virtual bool IsKeyDown(int key) const = 0;
  virtual float GetMouseWheelMove() const = 0;
};
