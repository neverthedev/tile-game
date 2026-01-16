#ifndef MENU_FACTORY_H
#define MENU_FACTORY_H

#include "menu.cpp"
#include "decoration_menu.cpp"
#include "../managers/tiles_manager.cpp"
#include "../input_components/menu/decoration_menu_component.cpp"
#include "../update_components/menu/decoration_menu_component.cpp"
#include "../graphics_components/menu/decoration_menu_component.cpp"

class MenuFactory {
  public:
    static DecorationMenu& CreateDecorationMenu(Rectangle2D, TilesManager&);
};

DecorationMenu& MenuFactory::CreateDecorationMenu(Rectangle2D pos, TilesManager& mngr) {
  DecorationMenuInputComponent* inp_cmp = new DecorationMenuInputComponent();
  DecorationMenuUpdateComponent* upd_cmp = new DecorationMenuUpdateComponent();
  DecorationMenuGraphicsComponent* grph_cmp = new DecorationMenuGraphicsComponent();

  DecorationMenu* decorationMenu = new DecorationMenu(pos, mngr, *inp_cmp, *upd_cmp, *grph_cmp);
  return *decorationMenu;
}

#endif // MENU_FACTORY_H
