#include "rectangle_2d.h"

Rectangle2D::Rectangle2D(float x_, float y_, float w_, float h_):
    x { x_ },
    y { y_ },
    width { w_ },
    height { h_ }
{}

Rectangle2D::Rectangle2D(Rectangle rect):
    Rectangle2D(rect.x, rect.y, rect.width, rect.height)
{}

Rectangle2D::~Rectangle2D() {}
