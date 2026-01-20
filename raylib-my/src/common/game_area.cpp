#include "game_area.h"

#include "../graphics/collision_system.h"
#include "../graphics/input_system.h"
#include "../graphics/render_system.h"

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

void GameArea::HandleInput(InputSystem& input, CollisionSystem& collision) {
    object.HandleInput(input, collision);
}

void GameArea::Update(CollisionSystem& collision) {
    object.Update(collision);
}

void GameArea::Render(RenderSystem& renderer) {
    object.Render(renderer);
}

GameArea::~GameArea() {}
