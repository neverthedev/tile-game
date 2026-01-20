#include "tile_component.h"
#include "../graphics/input_system.h"
#include "../graphics/collision_system.h"

TileInputComponent::TileInputComponent() {}

void TileInputComponent::HandleInput(GameObject& obj, InputSystem& input, CollisionSystem& collision) {
  // Tiles don't handle input currently
}

TileInputComponent::~TileInputComponent() {}
