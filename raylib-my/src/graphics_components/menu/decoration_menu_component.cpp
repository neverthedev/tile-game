#ifndef DECORATION_MENU_GRAPHICS_COMPONENT
#define DECORATION_MENU_GRAPHICS_COMPONENT

#include "../component.cpp"
#include "../../graphics.cpp"
#include "../../menus/decoration_menu.cpp"
#include "../../common/game_error.cpp"

class DecorationMenuGraphicsComponent: public GraphicsComponent {
  public:
    DecorationMenuGraphicsComponent(): GraphicsComponent() {};
    virtual void Render(GameObject&, Graphics&) override;
    ~DecorationMenuGraphicsComponent() override {};

  private:

};

void DecorationMenuGraphicsComponent::Render(GameObject& menu, Graphics& grph) {
  DecorationMenu *decorationMenu = dynamic_cast<DecorationMenu*>(&menu);
  if (!decorationMenu) throw GameError("Incorrect object type provided!");

  grph.DrawRectangle(decorationMenu->Position);
}

#endif // DECORATION_MENU_GRAPHICS_COMPONENT
