#ifndef COMMON_POSITION_2D
#define COMMON_POSITION_2D

#include "raylib.h"

class Position2D {
  public:
    Position2D(Vector2);
    Position2D(float x_, float y_): x { x_ }, y { y_ } {};
    Position2D(int x_, int y_): x { (float) x_ }, y { (float) y_ } {};
    operator const Vector2() const;
    Position2D& operator+=(const Position2D&);
    float x;  // Vector x component
    float y;  // Vector y component
};

Position2D::Position2D(Vector2 vect):
  x { vect.x },
  y { vect.y }
{};

Position2D& Position2D::operator+=(const Position2D& right) {
  x += right.x;
  y += right.y;
  return *this;
}

Position2D::operator const Vector2() const  {
  return Vector2 {x, y};
}

#endif // COMMON_POSITION_2D
