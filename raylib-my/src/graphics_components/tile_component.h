#pragma once

#include "component.h"

// Forward declaration
class WorldTile;

class TileGraphicsComponent: public GraphicsComponent {
public:
  TileGraphicsComponent();
  virtual void Render(GameObject&, RenderSystem&) override;
  ~TileGraphicsComponent() override;
};
