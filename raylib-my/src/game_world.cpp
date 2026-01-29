#include "game_world.h"
#include <string>
#include <utility>

#include "services/service_locator.h"
#include "services/tiles_manager.h"
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
  TilesProvider tilesProvider
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

void GameWorld::InitializeGrid(TilesProvider tilesProvider) {
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

#include "graphics_components/world_component.h"
#include "input_components/world_component.h"
#include "update_components/world_component.h"

std::unique_ptr<GameWorld> GameWorld::NewWorld(int w, int h) {
  return NewWorld(w, h, [w, h](int x, int y) {
    auto& tilesManager = ServiceLocator::GetTilesManager();
    const int oW = 3;
    const int gW = 10;

    if ((x < oW) || (y < oW) || (x > w - oW - 1) || (y > h - oW - 1)) {
      return std::unique_ptr<WorldTile>(tilesManager.NewTile("Deep Water", { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f }));
    }
    if ((x < gW) || (y < gW) || (x > w - gW - 1) || (y > h - gW - 1)) {
      return std::unique_ptr<WorldTile>(tilesManager.NewTile("Plains", { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f }));
    }
    return std::unique_ptr<WorldTile>(tilesManager.NewTile("Grassland", { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f }));
  });
}

std::unique_ptr<GameWorld> GameWorld::NewWorld(int w, int h, TilesProvider tile_provider) {
  return std::make_unique<GameWorld>(
    w, h,
    std::make_unique<WorldInputComponent>(),
    std::make_unique<WorldGraphicsComponent>(),
    std::make_unique<WorldUpdateComponent>(),
    std::move(tile_provider)
  );
}
