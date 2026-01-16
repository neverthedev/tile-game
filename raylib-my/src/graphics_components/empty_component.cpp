#include "empty_component.h"

#include "../graphics.h"

EmptyGraphicsComponent::EmptyGraphicsComponent(): GraphicsComponent() {}

void EmptyGraphicsComponent::Render(GameObject& wld, Graphics& graphics) {
  // Do nothing here
}

EmptyGraphicsComponent::~EmptyGraphicsComponent() {}
