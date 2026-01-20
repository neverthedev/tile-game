#pragma once

#include "rectangle_2d.h"
#include "game_object.h"

class GameArea: public GameObject {
public:
    GameArea(GameObject&, Rectangle2D, int);
    GameArea(const GameArea&) = delete;
    GameArea& operator=(const GameArea&) = delete;
    GameArea(GameArea&&) noexcept;
    GameArea& operator=(GameArea&&) = delete;
    virtual void HandleInput(InputSystem&, CollisionSystem&) override;
    virtual void Update(CollisionSystem&) override;
    virtual void Render(RenderSystem&) override;
    ~GameArea();

    Rectangle2D position;
    int priority;

private:
    GameObject& object;
};
