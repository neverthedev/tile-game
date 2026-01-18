#pragma once

#include <memory>
#include "menu.h"

class DecorationMenu: public Menu {
  friend class MenuFactory;

public:
  DecorationMenu(
    Rectangle2D,
    std::unique_ptr<InputComponent>,
    std::unique_ptr<UpdateComponent>,
    std::unique_ptr<GraphicsComponent>
  );
};
