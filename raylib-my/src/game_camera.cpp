#include "game_camera.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "update_components/component.h"

GameCamera::GameCamera(std::unique_ptr<InputComponent> inp, std::unique_ptr<GraphicsComponent> grph):
  GameObject(std::move(inp), std::move(grph)),
  offset { 0, 0 },
  target { 0, 0 },
  rotation { 0 },
  zoom { 1.0f }
{}

void GameCamera::Update() {
  // Camera has no own phisics
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

GameCamera::~GameCamera() = default;
