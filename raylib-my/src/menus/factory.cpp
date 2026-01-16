#include "factory.h"

#include "../input_components/menu/decoration_menu_component.h"
#include "../update_components/menu/decoration_menu_component.h"
#include "../graphics_components/menu/decoration_menu_component.h"

DecorationMenu& MenuFactory::CreateDecorationMenu(Rectangle2D pos, TilesManager& mngr) {
  DecorationMenuInputComponent* inp_cmp = new DecorationMenuInputComponent();
  DecorationMenuUpdateComponent* upd_cmp = new DecorationMenuUpdateComponent();
  DecorationMenuGraphicsComponent* grph_cmp = new DecorationMenuGraphicsComponent();

  DecorationMenu* decorationMenu = new DecorationMenu(pos, mngr, *inp_cmp, *upd_cmp, *grph_cmp);
  return *decorationMenu;
}
