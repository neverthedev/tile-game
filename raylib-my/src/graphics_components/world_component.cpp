#include "world_component.h"

#include "raylib.h"

#include "../common/game_error.h"
#include "../graphics.h"
#include "../game_world.h"
#include "../world_tiles/tile.h"

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

    for(int i = 0; i < world->MapHeight * world->MapWidth; ++i) {
      WorldTile& tile = world->GetTile(i);
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
