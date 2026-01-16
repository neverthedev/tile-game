#pragma once

#include "./component.h"

// Forward declarations
class GameObject;
class Graphics;

class EmptyGraphicsComponent: public GraphicsComponent {
public:
  EmptyGraphicsComponent();
  virtual void Render(GameObject&, Graphics&) override;
  ~EmptyGraphicsComponent() override;
};
