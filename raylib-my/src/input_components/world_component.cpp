#ifndef WORLD_INPUT_COMPONENT_H
#define WORLD_INPUT_COMPONENT_H

#include "component.h"
#include "../common/game_error.cpp"
#include "../common/game_object.cpp"
#include "../game_world.cpp"

class WorldInputComponent: public InputComponent {
  public:
    WorldInputComponent(): InputComponent() {};
    virtual void HandleInput(GameObject&) override;
    ~WorldInputComponent() override {};
};

void WorldInputComponent::HandleInput(GameObject& wld) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);

  if (!world) throw GameError("Incorrect object type provided!");
};

#endif // WORLD_INPUT_COMPONENT_H
