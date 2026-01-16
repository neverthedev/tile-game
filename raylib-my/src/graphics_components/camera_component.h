#pragma once

#include "./component.h"

// Forward declarations
class GameObject;
class Graphics;
class GameCamera;

class CameraGraphicsComponent: public GraphicsComponent {
public:
  CameraGraphicsComponent();
  virtual void Render(GameObject&, Graphics&) override;
  ~CameraGraphicsComponent() override;
};
