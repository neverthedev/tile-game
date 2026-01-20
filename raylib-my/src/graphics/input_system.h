#pragma once

#include "../common/position_2d.h"
#include "../common/keyboard_2d.h"

class InputSystem {
public:
  virtual ~InputSystem() = default;

  virtual Position2D GetMousePosition() const = 0;
  virtual bool IsKeyPressed(Keyboard2D::Key key) const = 0;
  virtual bool IsMouseButtonPressed(int button) const = 0;
  virtual bool IsKeyDown(Keyboard2D::Key key) const = 0;
  virtual float GetMouseWheelMove() const = 0;
};
