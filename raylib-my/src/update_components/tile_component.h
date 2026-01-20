#pragma once

#include "component.h"

// Forward declaration
class WorldTile;

class TileUpdateComponent: public UpdateComponent {
public:
  TileUpdateComponent();
  virtual void Update(GameObject&, CollisionSystem&) override;
  ~TileUpdateComponent() override;
};
