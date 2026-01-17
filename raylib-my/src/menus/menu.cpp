#include "menu.h"

Menu::Menu(
  Rectangle2D pos,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<UpdateComponent> upd,
  std::unique_ptr<GraphicsComponent> grph
):
  GameObject(std::move(inp), std::move(grph), std::move(upd)),
  Position { pos }
{}

Menu::~Menu() = default;
