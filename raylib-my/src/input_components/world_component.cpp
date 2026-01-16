#include "world_component.h"

#include "../common/game_error.h"
#include "../common/game_object.h"
#include "../game_world.h"

WorldInputComponent::WorldInputComponent(): InputComponent() {}

void WorldInputComponent::HandleInput(GameObject& wld) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);

  if (!world) throw GameError("Incorrect object type provided!");
}

WorldInputComponent::~WorldInputComponent() {}
