#include "game_camera.h"
#include "input_components/component.h"
#include "graphics_components/component.h"
#include "update_components/component.h"

GameCamera::GameCamera(std::unique_ptr<InputComponent> inp, std::unique_ptr<GraphicsComponent> grph, std::unique_ptr<UpdateComponent> upd):
  GameObject(std::move(inp), std::move(grph), std::move(upd)),
  offset { 0.0f, 0.0f },
  target { 0.0f, 0.0f },
  rotation { 0 },
  zoom { 1.0f }
{}

void GameCamera::UpdateFromGrphCamera(const GrphCamera& src) {
    offset = src.offset;
    target = src.target;
    rotation = src.rotation;
    zoom = src.zoom;
}

GameCamera::~GameCamera() = default;
