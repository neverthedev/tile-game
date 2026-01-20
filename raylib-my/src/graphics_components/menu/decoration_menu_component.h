#pragma once

#include "../component.h"

// Forward declarations
class GameObject;
class RenderSystem;
class DecorationMenu;

class DecorationMenuGraphicsComponent: public GraphicsComponent {
public:
  DecorationMenuGraphicsComponent();
  virtual void Render(GameObject&, RenderSystem&) override;
  ~DecorationMenuGraphicsComponent() override;
};
