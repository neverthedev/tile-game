#ifndef DECORATION_MENU_H
#define DECORATION_MENU_H

#include "menu.cpp"
#include "../managers/tiles_manager.cpp"

class DecorationMenu: public Menu {
  friend class MenuFactory;

  protected:
    DecorationMenu(Rectangle2D, TilesManager&, InputComponent&, UpdateComponent&, GraphicsComponent&);
  private:
    TilesManager& tilesManager;
};

DecorationMenu::DecorationMenu(Rectangle2D pos, TilesManager& mngr, InputComponent& c_inp, UpdateComponent& c_upd, GraphicsComponent& c_grph):
  Menu(pos, c_inp, c_upd, c_grph),
  tilesManager { mngr }
{

}

#endif // DECORATION_MENU_H
