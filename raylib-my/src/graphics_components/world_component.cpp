#include "world_component.h"

#include "../common/game_error.h"
#include "../common/color_2d.h"
#include "../graphics.h"
#include "../game_world.h"
#include "../world_tiles/tile.h"
#include "raylib.h"  // For raylib-specific functions still needed temporarily

WorldGraphicsComponent::WorldGraphicsComponent():
  GraphicsComponent(),
  initialized { false }
{}

void WorldGraphicsComponent::drawIsoTileFrame(Graphics& grph, Position2D pos) {
  Position2D center = grph.GridToScreen(pos);

  grph.DrawDiamondFrame(center, Color2D::Magenta(), false, 1.5f);  // MAGENTA
}

void WorldGraphicsComponent::Render(GameObject& wld, Graphics& grph) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);
  if (!world) throw GameError("Incorrect object type provided!");

  if (!world->IsInitialized()) {
    world->GetCamera().UpdateFromGrphCamera(grph.GetGrphCamera());

    world->SetInitialized();
  }
  world->GetCamera().Render(grph);

  Position2D gridF = grph.MouseToWorld2D();
  gridF += { 0.5f, 0.5f };

  ClearBackground(RAYWHITE);

  grph.BeginMode2D();
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
  grph.EndMode2D();
  DrawFPS(10, 10);
}

WorldGraphicsComponent::~WorldGraphicsComponent() {}
