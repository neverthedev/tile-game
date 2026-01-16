#include "decoration_menu_component.h"

#include "../../common/game_object.h"
#include "../../common/game_error.h"
#include "../../menus/decoration_menu.h"

DecorationMenuUpdateComponent::DecorationMenuUpdateComponent(): UpdateComponent() {}

void DecorationMenuUpdateComponent::Update(GameObject& menu) {
  DecorationMenu * decorationMenu = dynamic_cast<DecorationMenu *>(&menu);
  if (!decorationMenu) { throw GameError("Incorrect object type provided!"); }
}

DecorationMenuUpdateComponent::~DecorationMenuUpdateComponent() {}
