#include "camera_component.h"
#include "../graphics/collision_system.h"

CameraUpdateComponent::CameraUpdateComponent() {}

void CameraUpdateComponent::Update(GameObject& obj, CollisionSystem& collision) {
  // Camera has no own physics
}

CameraUpdateComponent::~CameraUpdateComponent() {}
