#pragma once

#include "../common/rectangle_2d.h"
#include "../graphics.h"
#include "../common/game_object.h"
#include "../input_components/component.h"
#include "../graphics_components/component.h"
#include "../update_components/component.h"

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
