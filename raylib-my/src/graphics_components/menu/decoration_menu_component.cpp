#include "decoration_menu_component.h"

#include "../../graphics.h"
#include "../../menus/decoration_menu.h"
#include "../../common/game_error.h"
#include "../../common/color_2d.h"

DecorationMenuGraphicsComponent::DecorationMenuGraphicsComponent(): GraphicsComponent() {}

void DecorationMenuGraphicsComponent::Render(GameObject& menu, Graphics& grph) {
  DecorationMenu *decorationMenu = dynamic_cast<DecorationMenu*>(&menu);
  if (!decorationMenu) throw GameError("Incorrect object type provided!");

  grph.DrawRectangle(decorationMenu->Position, Color2D::Black());
}

DecorationMenuGraphicsComponent::~DecorationMenuGraphicsComponent() {}
