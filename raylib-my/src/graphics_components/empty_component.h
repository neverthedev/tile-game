#pragma once

#include "./component.h"

// Forward declarations
class GameObject;
class RenderSystem;

class EmptyGraphicsComponent: public GraphicsComponent {
public:
  EmptyGraphicsComponent();
  virtual void Render(GameObject&, RenderSystem&) override;
  ~EmptyGraphicsComponent() override;
};
