#ifndef CAMERA_H
#define CAMERA_H

#include "raylib.h"

#include "graphics.cpp"
#include "common/game_object.cpp"
#include "input_components/component.h"
#include "graphics_components/component.cpp"

class GameCamera: public GameObject {
  public:
    const float ZOOM_SPEED = 0.1f;
    const float MOVE_SPEED = 3.0f;
    const float ZOOM_MIN = 0.5f;
    const float ZOOM_MAX = 3.0f;

    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default

    GameCamera(InputComponent &c, GraphicsComponent&);
    void HandleInput() override;
    void Update() override;
    void Render(Graphics&) override;
    void UpdateFromCamera2D(Camera2D);
    ~GameCamera();

    operator const Camera2D() const;
  private:
    InputComponent& input;
    GraphicsComponent& graphic;
};

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
#endif // CAMERA_H
