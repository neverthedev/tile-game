#pragma once

#include "menu.h"
#include "../managers/tiles_manager.h"

class DecorationMenu: public Menu {
  friend class MenuFactory;

protected:
  DecorationMenu(Rectangle2D, TilesManager&, InputComponent&, UpdateComponent&, GraphicsComponent&);

private:
  TilesManager& tilesManager;
};
