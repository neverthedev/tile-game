#include "game_world.h"

#include "input_components/camera_component.h"
#include "graphics_components/camera_component.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "update_components/component.h"

GameWorld::GameWorld(
  int w, int h,
  const TilesManager& tileMnr,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<GraphicsComponent> rnd
):
  GameObject(std::move(inp), std::move(rnd)),
  MapWidth { w },
  MapHeight { h },
  initialized { false },
  tilesManager { tileMnr },
  grid { }
{
  camera = new GameCamera(
    std::make_unique<CameraInputComponent>(),
    std::make_unique<CameraGraphicsComponent>()
  );

  initializeGrid();
};

void GameWorld::initializeGrid() {
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

void GameWorld::HandleInput() {
  GameObject::HandleInput();
  camera->HandleInput();
  for (auto& tile: grid) tile->HandleInput();
}

void GameWorld::Update() {
  for (auto& tile: grid) tile->Update();
};

void GameWorld::Render(Graphics& graphics) {
  if (!initialized) afterScreenInitialization(graphics);

  camera->Render(graphics);
  GameObject::Render(graphics);
};

void GameWorld::afterScreenInitialization(Graphics& grph) {
  if (initialized) return;

  camera->UpdateFromCamera2D(grph.camera);
  initialized = true;
}

const TilesManager& GameWorld::GetTilesManager() {
  return tilesManager;
}

GameWorld::~GameWorld() = default;

#include "graphics_components/world_component.h"
#include "input_components/world_component.h"

GameWorld GameWorld::NewWorld(int w, int h, const TilesManager& tileMngr) {
  return {
    w, h, tileMngr,
    std::make_unique<WorldInputComponent>(),
    std::make_unique<WorldGraphicsComponent>()
  };
}
