#include "decoration_menu.h"

DecorationMenu::DecorationMenu(Rectangle2D pos, TilesManager& mngr, InputComponent& c_inp, UpdateComponent& c_upd, GraphicsComponent& c_grph):
  Menu(pos, c_inp, c_upd, c_grph),
  tilesManager { mngr }
{

}
