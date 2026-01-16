#include "game_world.h"

#include "input_components/camera_component.h"
#include "graphics_components/camera_component.h"

GameWorld::GameWorld(int w, int h, const TilesManager& tileMnr, InputComponent& c_inp, GraphicsComponent& c_rnd):
  MapWidth { w },
  MapHeight { h },
  input { c_inp },
  render { c_rnd },
  initialized { false },
  tilesManager { tileMnr },
  grid { }
{
  CameraInputComponent * cameraInp = new CameraInputComponent;
  CameraGraphicsComponent * cameraGrph = new CameraGraphicsComponent;
  camera = new GameCamera(*cameraInp, *cameraGrph);

  initializeGrid();
};

void GameWorld::initializeGrid() {
  int oW = 3;
  int gW = 10;
  for (int h = 0; h < MapHeight; ++h) {
    for (int w  = 0; w < MapWidth; ++w) {
      if ((w < oW || w > MapWidth - oW - 1) || (h < oW || h > MapHeight - oW - 1)) {
        WorldTile& tile = *tilesManager.NewTile("Deep Water", { (float) w + 0.5f, (float) h + 0.5f });
        grid.push_back(tile);
      } else if ((w < gW || w > MapWidth - gW - 1) || (h < gW || h > MapHeight - gW - 1)) {
        WorldTile& tile = *tilesManager.NewTile("Plains", { (float) w + 0.5f, (float) h + 0.5f });
        grid.push_back(tile);
      } else {
        WorldTile& tile = *tilesManager.NewTile("Grassland", { (float) w + 0.5f, (float) h + 0.5f });
        grid.push_back(tile);
      }
    }
  }
}

WorldTile& GameWorld::operator[](Position2D pos) {
  if (pos.x >= MapWidth || pos.y >= MapHeight) {
    throw GameError("Grid possition overflow");
  }
  return grid[floor(pos.y) * MapWidth + floor(pos.x)];
}
WorldTile* GameWorld::begin() { return grid.data(); }
WorldTile* GameWorld::end() { return grid.data() + grid.size(); }

void GameWorld::HandleInput() {
  input.HandleInput(*this);
  camera->HandleInput();
  for (WorldTile& tile: *this) tile.HandleInput();
}

void GameWorld::Update() {
  for (WorldTile& tile: *this) tile.Update();
};

void GameWorld::Render(Graphics& graphics) {
  if (!initialized) afterScreenInitialization(graphics);

  camera->Render(graphics);
  render.Render(*this, graphics);
};

void GameWorld::afterScreenInitialization(Graphics& grph) {
  if (initialized) return;

  camera->UpdateFromCamera2D(grph.camera);
  initialized = true;
}

const TilesManager& GameWorld::GetTilesManager() {
  return tilesManager;
}

GameWorld::~GameWorld() {
  delete &input;
  delete &render;
}

#include "graphics_components/world_component.h"
#include "input_components/world_component.h"

GameWorld GameWorld::NewWorld(int w, int h, const TilesManager& tileMngr) {
  WorldInputComponent* ic = new WorldInputComponent{};
  WorldGraphicsComponent* gc = new WorldGraphicsComponent{};

  return { w, h, tileMngr, *ic, *gc };
}
