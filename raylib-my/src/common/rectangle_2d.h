#pragma once

class Rectangle2D {
public:
    float x;            // Rectangle top-left corner position x
    float y;            // Rectangle top-left corner position y
    float width;        // Rectangle width
    float height;       // Rectangle height

    Rectangle2D(float, float, float, float);
    ~Rectangle2D();
};
