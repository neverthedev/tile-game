#include "decoration_menu.h"

DecorationMenu::DecorationMenu(
  Rectangle2D pos,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<UpdateComponent> upd,
  std::unique_ptr<GraphicsComponent> grph,
  TilesManager& mngr
):
  Menu(pos, std::move(inp), std::move(upd), std::move(grph)),
  tilesManager { mngr }
{

}
