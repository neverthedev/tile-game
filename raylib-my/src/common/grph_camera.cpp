#include "grph_camera.h"

GrphCamera::GrphCamera():
  offset { 0.0f, 0.0f },
  target { 0.0f, 0.0f },
  rotation { 0.0f },
  zoom { 1.0f }
{}

GrphCamera::GrphCamera(Position2D offset_, Position2D target_, float rotation_, float zoom_):
  offset { offset_ },
  target { target_ },
  rotation { rotation_ },
  zoom { zoom_ }
{}

GrphCamera::~GrphCamera() = default;
