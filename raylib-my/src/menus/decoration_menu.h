#pragma once

#include <memory>
#include "menu.h"
#include "../managers/tiles_manager.h"

class DecorationMenu: public Menu {
  friend class MenuFactory;

protected:
  DecorationMenu(
    Rectangle2D,
    std::unique_ptr<InputComponent>,
    std::unique_ptr<UpdateComponent>,
    std::unique_ptr<GraphicsComponent>,
    TilesManager&
  );

private:
  TilesManager& tilesManager;
};
