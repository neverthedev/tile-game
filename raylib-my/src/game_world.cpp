#include "game_world.h"
#include <string>
#include <utility>

#include "input_components/camera_component.h"
#include "graphics_components/camera_component.h"
#include "update_components/camera_component.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "update_components/component.h"

GameWorld::GameWorld(
  int w, int h,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<GraphicsComponent> rnd,
  std::unique_ptr<UpdateComponent> upd,
  TileProvider tilesProvider
):
  GameObject(std::move(inp), std::move(rnd), std::move(upd)),
  MapWidth { w },
  MapHeight { h },
  grid { }
{
  camera = std::make_unique<GameCamera>(
    std::make_unique<CameraInputComponent>(),
    std::make_unique<CameraGraphicsComponent>(),
    std::make_unique<CameraUpdateComponent>()
  );

  InitializeGrid(std::move(tilesProvider));
};

void GameWorld::InitializeGrid(TileProvider tilesProvider) {
  for (int y = 0; y < MapHeight; ++y) {
    for (int x = 0; x < MapWidth; ++x) {
      std::unique_ptr<WorldTile> tile = tilesProvider(x, y);
      if (!tile) {
        throw GameError("Tile provider returned null tile for index x: " + std::to_string(x) + ", y: " + std::to_string(y));
      }
      grid.push_back(std::move(tile));
    }
  }
}

WorldTile& GameWorld::operator[](Position2D pos) {
  if (pos.x >= MapWidth || pos.y >= MapHeight) {
    throw GameError("Grid possition overflow");
  }
  return *grid[floor(pos.y) * MapWidth + floor(pos.x)];
}

WorldTile& GameWorld::GetTile(int index) {
  return *grid[index];
}

GameCamera& GameWorld::GetCamera() {
  return *camera;
}

GameWorld::~GameWorld() = default;
