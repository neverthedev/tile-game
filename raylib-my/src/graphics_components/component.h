#pragma once

// Forward declarations
class GameObject;
class RenderSystem;

class GraphicsComponent {
public:
    GraphicsComponent();
    virtual void Render(GameObject&, RenderSystem&) = 0;
    virtual ~GraphicsComponent();
};
