#pragma once

#include <memory>
#include "menu.h"
#include "decoration_menu.h"

class MenuFactory {
public:
  static std::unique_ptr<DecorationMenu> CreateDecorationMenu(Rectangle2D);
};
