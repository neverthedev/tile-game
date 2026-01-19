#include "component.h"

#include "raylib.h"
#include "raymath.h"
#include "../common/position_2d.h"

InputComponent::InputComponent() {}

bool InputComponent::IsKeyDown(int key) { // TODO: Consider switching to class/enum instead of int
  return ::IsKeyDown(key);
}

Position2D InputComponent::GetMousePosition() {
  Vector2 pos = ::GetMousePosition();
  return Position2D(pos.x, pos.y);
}

float InputComponent::GetMouseWheelMove() {
  return ::GetMouseWheelMove();
}

float InputComponent::Clamp(float value, float min, float max) {
  return ::Clamp(value, min, max);
}

InputComponent::~InputComponent() {}
