#pragma once

#include "./component.h"
#include "../common/image_handle.h"
#include "../common/texture_handle.h"

// Forward declarations
class GameObject;
class RenderSystem;
class GameWorld;
class Position2D;

class WorldGraphicsComponent: public GraphicsComponent {
public:
  WorldGraphicsComponent();
  virtual void Render(GameObject&, RenderSystem&) override;
  ~WorldGraphicsComponent() override;

private:
  void drawIsoTileFrame(RenderSystem&, Position2D);
  ImageHandle worldTileMap;
  TextureHandle mapTexture;
  bool initialized;
};
