#pragma once

#include "../component.h"

// Forward declarations
class GameObject;
class Graphics;
class DecorationMenu;

class DecorationMenuGraphicsComponent: public GraphicsComponent {
public:
  DecorationMenuGraphicsComponent();
  virtual void Render(GameObject&, Graphics&) override;
  ~DecorationMenuGraphicsComponent() override;
};
