#pragma once

#include "../common/rectangle_2d.h"
#include "../common/game_object.h"
#include "../input_components/component.h"
#include "../graphics_components/component.h"
#include "../update_components/component.h"

class MenuFactory;

class Menu: public GameObject {
  friend class MenuFactory;

public:
  Rectangle2D Position;

  ~Menu();

protected:
  Menu(Rectangle2D, std::unique_ptr<InputComponent>, std::unique_ptr<UpdateComponent>, std::unique_ptr<GraphicsComponent>);

private:
};
