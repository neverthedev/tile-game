#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "graphics/render_system.h"
#include "common/game_error.h"
#include "common/game_object.h"
#include "game_camera.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "world_tiles/tile.h"

class GameWorld: public GameObject {
public:
  int MapWidth;
  int MapHeight;

  using TilesProvider = std::function<std::unique_ptr<WorldTile>(int x, int y)>;

  static std::unique_ptr<GameWorld> NewWorld(int, int);
  static std::unique_ptr<GameWorld> NewWorld(int, int, TilesProvider);

  GameWorld(const GameWorld&) = delete;
  GameWorld& operator=(const GameWorld&) = delete;
  GameWorld(GameWorld&&) = delete;
  GameWorld& operator=(GameWorld&&) = delete;

  GameWorld(int, int, std::unique_ptr<InputComponent>, std::unique_ptr<GraphicsComponent>,
            std::unique_ptr<UpdateComponent>, TilesProvider);

  WorldTile& operator[](Position2D);
  WorldTile& GetTile(int);
  GameCamera& GetCamera();
  ~GameWorld();

private:
  std::unique_ptr<GameCamera> camera;
  std::vector<std::unique_ptr<WorldTile>> grid;
  void InitializeGrid(TilesProvider);
};
