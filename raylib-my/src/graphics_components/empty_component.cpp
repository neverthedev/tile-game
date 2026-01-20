#include "empty_component.h"

#include "../graphics/render_system.h"

EmptyGraphicsComponent::EmptyGraphicsComponent(): GraphicsComponent() {}

void EmptyGraphicsComponent::Render(GameObject& wld, RenderSystem& renderer) {
  // Do nothing here
}

EmptyGraphicsComponent::~EmptyGraphicsComponent() {}
