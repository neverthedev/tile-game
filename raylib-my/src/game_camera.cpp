#include "game_camera.h"

GameCamera::GameCamera(InputComponent& c_inp, GraphicsComponent& c_grph):
  input { c_inp },
  graphic { c_grph },
  offset { 0, 0 },
  target { 0, 0 },
  rotation { 0 },
  zoom { 1.0f }
{}

void GameCamera::HandleInput() {
  input.HandleInput(*this);
}

void GameCamera::Update() {
  // Camera has no own phisics
}

void GameCamera::Render(Graphics& grph) {
  graphic.Render(*this, grph);
}

void GameCamera::UpdateFromCamera2D(Camera2D src) {
  offset = src.offset;
  zoom = src.zoom;
  target = src.target;
  rotation = src.rotation;
}

GameCamera::operator const Camera2D() const {
  return Camera2D { offset, target, rotation, zoom };
}

GameCamera::~GameCamera() {
  delete &input;
  delete &graphic;
}
