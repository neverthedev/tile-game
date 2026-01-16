#pragma once

#include "raylib.h"

#include "./component.h"

// Forward declarations
class GameObject;
class Graphics;
class GameWorld;
class Position2D;

class WorldGraphicsComponent: public GraphicsComponent {
public:
  WorldGraphicsComponent();
  virtual void Render(GameObject&, Graphics&) override;
  ~WorldGraphicsComponent() override;

private:
  void drawIsoTileFrame(Graphics&, Position2D);
  Image worldTileMap;
  Texture2D mapTexture;
  bool initialized;
};
