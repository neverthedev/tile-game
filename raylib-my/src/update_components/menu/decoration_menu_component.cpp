#ifndef DECORATION_MENU_UPDATE_COMPONENT
#define DECORATION_MENU_UPDATE_COMPONENT

#include "../component.cpp"
#include "../../common/game_object.cpp"
#include "../../common/game_error.cpp"
#include "../../menus/decoration_menu.cpp"

class DecorationMenuUpdateComponent: public UpdateComponent {
  public:
    DecorationMenuUpdateComponent();
    void virtual Update(GameObject&) override;
    ~DecorationMenuUpdateComponent();
};

DecorationMenuUpdateComponent::DecorationMenuUpdateComponent(): UpdateComponent() {}

void DecorationMenuUpdateComponent::Update(GameObject& menu) {
  DecorationMenu * decorationMenu = dynamic_cast<DecorationMenu *>(&menu);
  if (!decorationMenu) { throw GameError("Incorrect object type provided!"); }
}

DecorationMenuUpdateComponent::~DecorationMenuUpdateComponent() {}

#endif // DECORATION_MENU_UPDATE_COMPONENT
