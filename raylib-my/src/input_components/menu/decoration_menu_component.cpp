#include "decoration_menu_component.h"

#include "../../common/game_object.h"
#include "../../common/game_error.h"
#include "../../menus/decoration_menu.h"
#include "../../graphics/input_system.h"
#include "../../graphics/collision_system.h"

DecorationMenuInputComponent::DecorationMenuInputComponent(): InputComponent() {}

void DecorationMenuInputComponent::HandleInput(GameObject& menu, InputSystem& input, CollisionSystem& collision) {
  DecorationMenu * decorationMenu = dynamic_cast<DecorationMenu *>(&menu);
  if (!decorationMenu) { throw GameError("Incorrect object type provided!"); }

}

DecorationMenuInputComponent::~DecorationMenuInputComponent() {}
