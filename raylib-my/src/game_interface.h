#pragma once

#include <vector>

#include "raylib.h"

#include "common/game_area.h"
#include "managers/tiles_manager.h"
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
  void LoadTextures(Graphics& grph);

  ~GameInterface();

private:
  int screenWidth;
  int screenHeight;
  std::vector<GameArea*> gameAreas;
  TilesManager tilesManager;
  GameWorld gameWorld;
  Menu* currentMenu;
};
