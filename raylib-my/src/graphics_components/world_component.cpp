#include "world_component.h"

#include "../common/color_2d.h"
#include "../common/game_error.h"
#include "../game_world.h"
#include "../graphics/render_system.h"
#include "../world_tiles/tile.h"

WorldGraphicsComponent::WorldGraphicsComponent():
  GraphicsComponent(),
  initialized { false }
{}

void WorldGraphicsComponent::drawIsoTileFrame(RenderSystem& renderer, Position2D pos) {
  Position2D center = renderer.GridToScreen(pos);

  renderer.DrawDiamondFrame(center, Color2D::Magenta(), false, 1.5f);  // MAGENTA
}

void WorldGraphicsComponent::Render(GameObject& wld, RenderSystem& renderer) {
  GameWorld* world = dynamic_cast<GameWorld*>(&wld);
  if (!world) throw GameError("Incorrect object type provided!");

  if (!world->IsInitialized()) {
    world->GetCamera().UpdateFromGrphCamera(renderer.GetGrphCamera());
    world->SetInitialized();
  }
  world->GetCamera().Render(renderer);

  Position2D gridF = renderer.MouseToWorld2D();
  gridF += { 0.5f, 0.5f };

  renderer.ClearBackground(Color2D(245, 245, 245, 255));  // RAYWHITE

  renderer.BeginMode2D();
    if (!initialized) {
      float tileWidth = renderer.GetTileWidth();
      float tileHeight = renderer.GetTileHeight();
      float width =  0.5 * tileWidth * (world->MapWidth + world->MapHeight);
      float height = 0.5 * tileHeight * (world->MapWidth + world->MapHeight);

      worldTileMap = renderer.GenImageColor(width, height, Color2D(0, 0, 0, 0));
      initialized = true;
    }

    bool redraw = false;
    float tileWidth = renderer.GetTileWidth();
    renderer.SetDst(worldTileMap);
    renderer.SetCorrection(Position2D(world->MapHeight * tileWidth / 2.0f, 0.0f));

    for(int i = 0; i < world->MapHeight * world->MapWidth; ++i) {
      WorldTile& tile = world->GetTile(i);
      if (tile.Dirty) {
        tile.Render(renderer);
        redraw |= true;
      }
    }

    if (redraw) {
      if (mapTexture.IsValid()) {
        renderer.UnloadTexture(mapTexture);
      }
      mapTexture = renderer.LoadTextureFromImage(worldTileMap);
    }

  Position2D correction = renderer.GetCorrection();
  renderer.DrawTexture(mapTexture, Position2D(-correction.x, -correction.y), Color2D(255, 255, 255, 255));  // WHITE
  drawIsoTileFrame(renderer, gridF);
  renderer.EndMode2D();
  renderer.DrawFPS(10, 10);
}

WorldGraphicsComponent::~WorldGraphicsComponent() {}
