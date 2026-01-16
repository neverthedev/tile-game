#pragma once

#include "raylib.h"

class Rectangle2D {
public:
    float x;            // Rectangle top-left corner position x
    float y;            // Rectangle top-left corner position y
    float width;        // Rectangle width
    float height;       // Rectangle height

    Rectangle2D(Rectangle);
    Rectangle2D(float, float, float, float);
    ~Rectangle2D();
};
