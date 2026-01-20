#include "game_object.h"

#include "../graphics/collision_system.h"
#include "../graphics/input_system.h"
#include "../graphics/render_system.h"
#include "../graphics_components/component.h"
#include "../input_components/component.h"
#include "../update_components/component.h"

GameObject::GameObject() {}

GameObject::GameObject(
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<GraphicsComponent> grph,
  std::unique_ptr<UpdateComponent> upd
):
  inputComponent { std::move(inp) },
  graphicsComponent { std::move(grph) },
  updateComponent { std::move(upd) }
{}

void GameObject::HandleInput(InputSystem& input, CollisionSystem& collision) {
  if (inputComponent) {
    inputComponent->HandleInput(*this, input, collision);
  }
}

void GameObject::Update(CollisionSystem& collision) {
  if (updateComponent) {
    updateComponent->Update(*this, collision);
  }
}

void GameObject::Render(RenderSystem& renderer) {
  if (graphicsComponent) {
    graphicsComponent->Render(*this, renderer);
  }
}

GameObject::~GameObject() = default;
