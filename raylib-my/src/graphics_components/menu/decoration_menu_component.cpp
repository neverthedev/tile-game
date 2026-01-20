#include "decoration_menu_component.h"

#include "../../graphics/render_system.h"
#include "../../menus/decoration_menu.h"
#include "../../common/game_error.h"
#include "../../common/color_2d.h"

DecorationMenuGraphicsComponent::DecorationMenuGraphicsComponent(): GraphicsComponent() {}

void DecorationMenuGraphicsComponent::Render(GameObject& menu, RenderSystem& renderer) {
  DecorationMenu *decorationMenu = dynamic_cast<DecorationMenu*>(&menu);
  if (!decorationMenu) throw GameError("Incorrect object type provided!");

  renderer.DrawRectangle(decorationMenu->Position, Color2D::Black());
}

DecorationMenuGraphicsComponent::~DecorationMenuGraphicsComponent() {}
