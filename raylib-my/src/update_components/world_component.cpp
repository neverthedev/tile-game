#include "world_component.h"

#include "../common/game_error.h"
#include "../common/game_object.h"
#include "../game_world.h"
#include "../graphics/collision_system.h"

WorldUpdateComponent::WorldUpdateComponent(): UpdateComponent() {}

void WorldUpdateComponent::Update(GameObject& wld, CollisionSystem& collision) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);

  if (!world) throw GameError("Incorrect object type provided!");

  for (int i = 0; i < world->MapWidth * world->MapHeight; ++i) {
    world->GetTile(i).Update(collision);
  }
}

WorldUpdateComponent::~WorldUpdateComponent() {}
