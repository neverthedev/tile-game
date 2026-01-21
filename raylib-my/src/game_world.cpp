#include "game_world.h"
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
  std::unique_ptr<UpdateComponent> upd
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

  initializeGrid();
};

void GameWorld::initializeGrid() {
  auto& tilesManager = ServiceLocator::GetTilesManager();
  int oW = 3;
  int gW = 10;
  for (int h = 0; h < MapHeight; ++h) {
    for (int w  = 0; w < MapWidth; ++w) {
      if ((w < oW || w > MapWidth - oW - 1) || (h < oW || h > MapHeight - oW - 1)) {
        std::unique_ptr<WorldTile> tile(tilesManager.NewTile("Deep Water", { (float) w + 0.5f, (float) h + 0.5f }));
        grid.push_back(std::move(tile));
      } else if ((w < gW || w > MapWidth - gW - 1) || (h < gW || h > MapHeight - gW - 1)) {
        std::unique_ptr<WorldTile> tile(tilesManager.NewTile("Plains", { (float) w + 0.5f, (float) h + 0.5f }));
        grid.push_back(std::move(tile));
      } else {
        std::unique_ptr<WorldTile> tile(tilesManager.NewTile("Grassland", { (float) w + 0.5f, (float) h + 0.5f }));
        grid.push_back(std::move(tile));
      }
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

GameWorld GameWorld::NewWorld(int w, int h) {
  return {
    w, h,
    std::make_unique<WorldInputComponent>(),
    std::make_unique<WorldGraphicsComponent>(),
    std::make_unique<WorldUpdateComponent>()
  };
}
