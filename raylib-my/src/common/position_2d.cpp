#include "position_2d.h"

Position2D::Position2D(float x_, float y_):
    x { x_ },
    y { y_ }
{}

Position2D::Position2D(int x_, int y_):
    x { (float) x_ },
    y { (float) y_ }
{}

Position2D& Position2D::operator+=(const Position2D& right) {
    x += right.x;
    y += right.y;
    return *this;
}
