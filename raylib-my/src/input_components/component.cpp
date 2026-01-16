#include "component.h"

#include "raylib.h"
#include "raymath.h"

InputComponent::InputComponent() {}

bool InputComponent::IsKeyDown(KeyboardKey key) {
  return ::IsKeyDown(key);
}

Position2D InputComponent::GetMousePosition() {
  return (Position2D) ::GetMousePosition();
}

float InputComponent::GetMouseWheelMove() {
  return ::GetMouseWheelMove();
}

float InputComponent::Clamp(float value, float min, float max) {
  return ::Clamp(value, min, max);
}

InputComponent::~InputComponent() {}
