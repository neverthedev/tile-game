#ifndef COMMON_GAME_OBJECT_H
#define COMMON_GAME_OBJECT_H

#include "../graphics.cpp"

class GameObject {
  public:
    GameObject();
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Render(Graphics&) = 0;
    virtual ~GameObject();
};

GameObject::GameObject() {};
GameObject::~GameObject() {};

#endif // COMMON_GAME_OBJECT_H
