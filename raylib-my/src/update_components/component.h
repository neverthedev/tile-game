#pragma once

#include "../common/game_object.h"

class UpdateComponent {
public:
    UpdateComponent();
    virtual void Update(GameObject&) = 0;
    virtual ~UpdateComponent();
};
