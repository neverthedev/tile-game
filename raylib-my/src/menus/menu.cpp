#ifndef MENU_H
#define MENU_H

#include "../common/rectangle_2d.cpp"
#include "../graphics.cpp"
#include "../common/game_object.cpp"
#include "../input_components/component.h"
#include "../graphics_components/component.cpp"
#include "../update_components/component.cpp"

class MenuFactory;
class Menu: public GameObject {
  friend class MenuFactory;

  public:
    Rectangle2D Position;

    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Render(Graphics& grph) override;

    ~Menu();
  protected:
    Menu(Rectangle2D, InputComponent&, UpdateComponent&, GraphicsComponent&);
  private:
    InputComponent& input;
    UpdateComponent& update;
    GraphicsComponent& graphics;
};

Menu::Menu(Rectangle2D pos, InputComponent& c_inp, UpdateComponent& c_upd, GraphicsComponent& c_grph):
  Position { pos },
  input { c_inp },
  update { c_upd },
  graphics { c_grph }
{}

void Menu::HandleInput() { input.HandleInput(*this); }
void Menu::Update() { update.Update(*this); }
void Menu::Render(Graphics& grph) { graphics.Render(*this, grph); }

Menu::~Menu() {
  delete &input;
  delete &graphics;
}

#endif // MENU_H
