#pragma once

#include "position_2d.h"

class GrphCamera {
public:
  Position2D offset;      // Camera offset (displacement from target)
  Position2D target;      // Camera target (rotation and zoom origin)
  float rotation;         // Camera rotation in degrees
  float zoom;             // Camera zoom (scaling), should be 1.0f by default

  GrphCamera();
  GrphCamera(Position2D offset, Position2D target, float rotation, float zoom);
  ~GrphCamera();
};
