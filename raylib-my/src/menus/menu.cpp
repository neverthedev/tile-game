#pragma once

#include "menu.h"

Menu::Menu(Rectangle2D pos, InputComponent& c_inp, UpdateComponent& c_upd, GraphicsComponent& c_grph):
    Position { pos },
    input { c_inp },
    update { c_upd },
    graphics { c_grph }
{}

void Menu::HandleInput() {
    input.HandleInput(*this);
}

void Menu::Update() {
    update.Update(*this);
}

void Menu::Render(Graphics& grph) {
    graphics.Render(*this, grph);
}

Menu::~Menu() {
    delete &input;
    delete &graphics;
}
