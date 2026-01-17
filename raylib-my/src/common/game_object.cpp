#include "game_object.h"
#include "../input_components/component.h"
#include "../graphics_components/component.h"
#include "../update_components/component.h"
#include "../graphics.h"

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

void GameObject::HandleInput() {
  if (inputComponent) {
    inputComponent->HandleInput(*this);
  }
}

void GameObject::Update() {
  if (updateComponent) {
    updateComponent->Update(*this);
  }
}

void GameObject::Render(Graphics& grph) {
  if (graphicsComponent) {
    graphicsComponent->Render(*this, grph);
  }
}

GameObject::~GameObject() = default;
