#pragma once

#include <vector>
#include <map>
#include <memory>

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

  GameWorld(int w, int h, const TilesManager&, std::unique_ptr<InputComponent>, std::unique_ptr<GraphicsComponent>);
  WorldTile& operator[](Position2D);
  WorldTile& GetTile(int index);

  void HandleInput() override;
  void Update() override;
  void Render(Graphics&) override;
  const TilesManager& GetTilesManager();
  ~GameWorld();

private:
  GameCamera* camera;
  bool initialized;
  std::vector<std::unique_ptr<WorldTile>> grid;
  const TilesManager& tilesManager;

  void initializeGrid();
  void initializeTyleTypes();

  void afterScreenInitialization(Graphics& graphics);
};
