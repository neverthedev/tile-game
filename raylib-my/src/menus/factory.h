#pragma once

#include "menu.h"
#include "decoration_menu.h"
#include "../managers/tiles_manager.h"

class MenuFactory {
public:
  static DecorationMenu& CreateDecorationMenu(Rectangle2D, TilesManager&);
};
