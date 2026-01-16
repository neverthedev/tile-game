#pragma once

// Forward declaration
class Graphics;

class GameObject {
public:
    GameObject();
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Render(Graphics&) = 0;
    virtual ~GameObject();
};
