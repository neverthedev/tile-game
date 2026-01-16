#pragma once

// Forward declarations
class GameObject;
class Graphics;

class GraphicsComponent {
public:
    GraphicsComponent();
    virtual void Render(GameObject&, Graphics&) = 0;
    virtual ~GraphicsComponent();
};
