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
  Position2D mouse = input.GetMousePosition();
  int hoveredIndex = -1;

  const auto& items = decorationMenu->Items();
  const int itemCount = static_cast<int>(items.size());
  for (int i = 0; i < itemCount; ++i) {
    Rectangle2D rowRect = decorationMenu->ItemRect(i);
    if (collision.CheckCollisionPointRec(mouse, rowRect)) {
      hoveredIndex = i;
      break;
    }
  }

  decorationMenu->SetHoveredIndex(hoveredIndex);

  constexpr int kMouseLeftButton = 0;
  if (hoveredIndex >= 0 && input.IsMouseButtonPressed(kMouseLeftButton)) {
    decorationMenu->SetSelectedIndex(hoveredIndex);
  }

}

DecorationMenuInputComponent::~DecorationMenuInputComponent() {}
