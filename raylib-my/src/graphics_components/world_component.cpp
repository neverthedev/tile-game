#ifndef WORLD_GRAPHICS_COMPONENT
#define WORLD_GRAPHICS_COMPONENT

#include "raylib.h"
#include "../common/game_error.cpp"
#include "./component.cpp"
#include "../graphics.cpp"
#include "../game_world.cpp"
#include "../world_tiles/tile.cpp"

class GameWorld;
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

WorldGraphicsComponent::WorldGraphicsComponent():
  GraphicsComponent(),
  initialized { false }
{}

void WorldGraphicsComponent::drawIsoTileFrame(Graphics& grph, Position2D pos) {
  Vector2 center = grph.GridToScreen(pos);

  grph.DrawDiamondFrame(center, MAGENTA, false, 1.5f);
}

void WorldGraphicsComponent::Render(GameObject& wld, Graphics& grph) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);
  if (!world) throw GameError("Incorrect object type provided!");

  Position2D gridF = grph.MouseToWorld2D();
  gridF += { 0.5f, 0.5f };

  ClearBackground(RAYWHITE);

  BeginMode2D(grph.camera);
    if (!initialized) {
      float width =  0.5 * grph.TileWidth * (world->MapWidth + world->MapHeight);
      float height = 0.5 * grph.TileHeight * (world->MapWidth + world->MapHeight);

      worldTileMap = GenImageColor(width, height, BLANK);
      initialized = true;
    }

    bool redraw = false;
    grph.Dst = &worldTileMap;
    grph.Correction = Position2D(world->MapHeight * grph.TileWidth / 2.0f, 0.0f);

    for(WorldTile& tile: *world) {
      if (tile.Dirty) {
        tile.Render(grph);
        redraw |= true;
      }
    }

    if (redraw) {
      mapTexture = LoadTextureFromImage(worldTileMap);
    } else {
    }

  DrawTexture(mapTexture, -grph.Correction.x, -grph.Correction.y, WHITE);
  drawIsoTileFrame(grph, gridF);
  EndMode2D();
  DrawFPS(10, 10);
}

WorldGraphicsComponent::~WorldGraphicsComponent() {}

#endif // WORLD_GRAPHICS_COMPONENT
