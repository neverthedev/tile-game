#pragma once

#include <vector>

#include "common/game_area.h"
#include "game_world.h"
#include "menus/menu.h"
#include "menus/factory.h"

class GameInterface: public GameObject {
public:
  GameInterface(int, int);
  virtual void HandleInput(InputSystem&, CollisionSystem&) override;
  virtual void Update(CollisionSystem&) override;
  virtual void Render(RenderSystem&) override;

  ~GameInterface();

private:
  int screenWidth;
  int screenHeight;
  std::vector<GameArea> gameAreas;
  std::vector<size_t> sortedIndices;
  GameWorld gameWorld;
  std::unique_ptr<Menu> currentMenu;

  void AddArea(GameObject&, Rectangle2D, int);
};
