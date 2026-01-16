#pragma once

#include "raylib.h"

class Position2D {
public:
    Position2D(Vector2);
    Position2D(float x_, float y_);
    Position2D(int x_, int y_);
    operator const Vector2() const;
    Position2D& operator+=(const Position2D&);

    float x;  // Vector x component
    float y;  // Vector y component
};
