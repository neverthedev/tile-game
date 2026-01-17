#include "game_area.h"

#include "../graphics.h"

GameArea::GameArea(GameObject& obj, Rectangle2D pos, int prt):
    GameObject(),
    position { pos },
    object { obj },
    priority { prt }
{}

GameArea::GameArea(GameArea&& other) noexcept:
    GameObject(),
    position { other.position },
    object { other.object },
    priority { other.priority }
{}

void GameArea::HandleInput() {
    object.HandleInput();
}

void GameArea::Update() {
    object.Update();
}

void GameArea::Render(Graphics& grph) {
    object.Render(grph);
}

GameArea::~GameArea() {}
