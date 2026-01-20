#pragma once

#include "../common/game_object.h"
#include "../common/position_2d.h"

// Forward declarations
class InputSystem;
class CollisionSystem;

class InputComponent {
public:
    InputComponent();
    virtual void HandleInput(GameObject&, InputSystem&, CollisionSystem&) = 0;
    virtual ~InputComponent();

protected:
    float Clamp(float, float, float);
};
