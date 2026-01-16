#pragma once

#include <vector>
#include <map>

#include "graphics.h"
#include "common/game_error.h"
#include "common/game_object.h"
#include "managers/tiles_manager.h"
#include "game_camera.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "world_tiles/tile.h"

class GameWorld: public GameObject {
public:
  int MapWidth;
  int MapHeight;

  static GameWorld NewWorld(int, int, const TilesManager&);

  GameWorld(int w, int h, const TilesManager&, InputComponent&, GraphicsComponent&);
  WorldTile& operator[](Position2D);
  WorldTile* begin();
  WorldTile* end();

  void HandleInput() override;
  void Update() override;
  void Render(Graphics&) override;
  const TilesManager& GetTilesManager();
  ~GameWorld();

private:
  InputComponent& input;
  GraphicsComponent& render;
  GameCamera* camera;
  bool initialized;
  std::vector<WorldTile> grid;
  const TilesManager& tilesManager;

  void initializeGrid();
  void initializeTyleTypes();

  void afterScreenInitialization(Graphics& graphics);
};
