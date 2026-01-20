#include "component.h"

#include <algorithm>

InputComponent::InputComponent() {}

float InputComponent::Clamp(float value, float min, float max) {
  return std::clamp(value, min, max);
}

InputComponent::~InputComponent() {}
