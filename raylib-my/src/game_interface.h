#pragma once

#include <vector>

#include "raylib.h"

#include "common/game_area.h"
#include "game_world.h"
#include "menus/menu.h"
#include "menus/factory.h"

class GameInterface: public GameObject {
public:
  GameInterface(int, int);
  void AddArea(GameObject&, Rectangle2D, int);
  virtual void HandleInput() override;
  virtual void Update() override;
  virtual void Render(Graphics& grph) override;

  ~GameInterface();

private:
  int screenWidth;
  int screenHeight;
  std::vector<GameArea> gameAreas;
  std::vector<size_t> sortedIndices;
  GameWorld gameWorld;
  std::unique_ptr<Menu> currentMenu;
};
