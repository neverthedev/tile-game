#include "world_component.h"

#include "../common/game_error.h"
#include "../common/game_object.h"
#include "../game_world.h"
#include "../graphics/input_system.h"
#include "../graphics/collision_system.h"

WorldInputComponent::WorldInputComponent(): InputComponent() {}

void WorldInputComponent::HandleInput(GameObject& wld, InputSystem& input, CollisionSystem& collision) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);

  if (!world) throw GameError("Incorrect object type provided!");

  world->GetCamera().HandleInput(input, collision);

  for (int i = 0; i < world->MapWidth * world->MapHeight; ++i) {
    world->GetTile(i).HandleInput(input, collision);
  }
}

WorldInputComponent::~WorldInputComponent() {}
