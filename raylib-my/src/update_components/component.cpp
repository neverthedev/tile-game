#ifndef UPDATE_COMPONENT_H
#define UPDATE_COMPONENT_H

#include "../common/game_object.cpp"

class UpdateComponent {
  public:
    UpdateComponent();
    virtual void Update(GameObject&) = 0;
    virtual ~UpdateComponent();
};

UpdateComponent::UpdateComponent() {}
UpdateComponent::~UpdateComponent() {}

#endif // UPDATE_COMPONENT
