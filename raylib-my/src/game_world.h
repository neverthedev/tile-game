#pragma once

#include <vector>
#include <map>
#include <memory>

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

  static GameWorld NewWorld(int, int);

  GameWorld(int w, int h, std::unique_ptr<InputComponent>, std::unique_ptr<GraphicsComponent>, std::unique_ptr<UpdateComponent>);
  WorldTile& operator[](Position2D);
  WorldTile& GetTile(int index);
  GameCamera& GetCamera();
  bool IsInitialized() const;
  void SetInitialized();
  ~GameWorld();

private:
  std::unique_ptr<GameCamera> camera;
  bool initialized;
  std::vector<std::unique_ptr<WorldTile>> grid;

  void initializeGrid();
  void initializeTyleTypes();
};
