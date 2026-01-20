#pragma once

#include "../component.h"

// Forward declarations
class GameObject;
class DecorationMenu;

class DecorationMenuUpdateComponent: public UpdateComponent {
public:
  DecorationMenuUpdateComponent();
  void virtual Update(GameObject&, CollisionSystem&) override;
  ~DecorationMenuUpdateComponent();
};
