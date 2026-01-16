#ifndef DECORATION_MENU_INPUT_COMPONENT
#define DECORATION_MENU_INPUT_COMPONENT

#include "../component.cpp"
#include "../../common/game_object.cpp"
#include "../../common/game_error.cpp"
#include "../../menus/decoration_menu.cpp"

class DecorationMenuInputComponent: public InputComponent {
  public:
    DecorationMenuInputComponent();
    void virtual HandleInput(GameObject&) override;
    ~DecorationMenuInputComponent();
};

DecorationMenuInputComponent::DecorationMenuInputComponent(): InputComponent() {}

void DecorationMenuInputComponent::HandleInput(GameObject& menu) {
  DecorationMenu * decorationMenu = dynamic_cast<DecorationMenu *>(&menu);
  if (!decorationMenu) { throw GameError("Incorrect object type provided!"); }

}

DecorationMenuInputComponent::~DecorationMenuInputComponent() {}

#endif // DECORATION_MENU_INPUT_COMPONENT
