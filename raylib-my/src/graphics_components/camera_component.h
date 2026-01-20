#pragma once

#include "./component.h"

// Forward declarations
class GameObject;
class RenderSystem;
class GameCamera;

class CameraGraphicsComponent: public GraphicsComponent {
public:
  CameraGraphicsComponent();
  virtual void Render(GameObject&, RenderSystem&) override;
  ~CameraGraphicsComponent() override;
};
