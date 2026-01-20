#include "tile_component.h"
#include "../graphics/collision_system.h"

TileUpdateComponent::TileUpdateComponent() {}

void TileUpdateComponent::Update(GameObject& obj, CollisionSystem& collision) {
  // Tiles don't update currently
  // For example, grow the tree
}

TileUpdateComponent::~TileUpdateComponent() {}
