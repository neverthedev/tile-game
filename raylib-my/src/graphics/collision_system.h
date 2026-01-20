#pragma once

#include "../common/position_2d.h"
#include "../common/rectangle_2d.h"

class CollisionSystem {
public:
  virtual ~CollisionSystem() = default;

  virtual bool CheckCollisionPointRec(Position2D point, Rectangle2D rect) const = 0;
  virtual bool CheckCollisionRecs(Rectangle2D rec1, Rectangle2D rec2) const = 0;
};
