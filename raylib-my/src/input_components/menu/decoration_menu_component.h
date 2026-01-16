#pragma once

#include "../component.h"

// Forward declarations
class GameObject;
class DecorationMenu;

class DecorationMenuInputComponent: public InputComponent {
public:
  DecorationMenuInputComponent();
  void virtual HandleInput(GameObject&) override;
  ~DecorationMenuInputComponent();
};
