#pragma once

#include "component.h"

// Forward declarations
class GameObject;
class GameWorld;

class WorldUpdateComponent: public UpdateComponent {
public:
  WorldUpdateComponent();
  virtual void Update(GameObject&, CollisionSystem&) override;
  ~WorldUpdateComponent() override;
};
