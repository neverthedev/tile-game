#ifndef COMMON_RECTANGLE_2D
#define COMMON_RECTANGLE_2D

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

Rectangle2D::Rectangle2D(float x_, float y_, float w_, float h_):
  x { x_ },
  y { y_ },
  width { w_ },
  height { h_ }
{}

Rectangle2D::Rectangle2D(Rectangle rect): Rectangle2D(rect.x, rect.y, rect.width, rect.height) {}

Rectangle2D::~Rectangle2D() {}

#endif // COMMON_RECTANGLE_2D
