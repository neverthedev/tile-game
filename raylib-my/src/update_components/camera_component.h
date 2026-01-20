#pragma once

#include "component.h"

class CameraUpdateComponent: public UpdateComponent {
public:
  CameraUpdateComponent();
  virtual void Update(GameObject&, CollisionSystem&) override;
  ~CameraUpdateComponent() override;
};
