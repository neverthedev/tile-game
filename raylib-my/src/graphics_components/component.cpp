#ifndef GRAPHICS_COMPONENT
#define GRAPHICS_COMPONENT

#include "../graphics.cpp"
#include "../common/game_object.cpp"

class GraphicsComponent {
  public:
    GraphicsComponent();
    virtual void Render(GameObject&, Graphics&) = 0;
    virtual ~GraphicsComponent();
};

GraphicsComponent::GraphicsComponent() {}
GraphicsComponent::~GraphicsComponent() {}

#endif // GRAPHICS_COMPONENT
