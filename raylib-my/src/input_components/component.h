#pragma once

#include "../common/game_object.h"
#include "../common/position_2d.h"

class InputComponent {
public:
    InputComponent();
    virtual void HandleInput(GameObject&) = 0;
    virtual ~InputComponent();

protected:
    bool IsKeyDown(int key);
    Position2D GetMousePosition();
    float GetMouseWheelMove();
    float Clamp(float, float, float);
};
