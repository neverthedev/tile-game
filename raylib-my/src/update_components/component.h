#pragma once

#include "../common/game_object.h"

// Forward declaration
class CollisionSystem;

class UpdateComponent {
public:
    UpdateComponent();
    virtual void Update(GameObject&, CollisionSystem&) = 0;
    virtual ~UpdateComponent();
};
