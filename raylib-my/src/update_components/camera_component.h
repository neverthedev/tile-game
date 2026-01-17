#pragma once

#include "component.h"

class CameraUpdateComponent: public UpdateComponent {
public:
  CameraUpdateComponent();
  virtual void Update(GameObject&) override;
  ~CameraUpdateComponent() override;
};
