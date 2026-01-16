#ifndef EMPTY_GRAPHICS_COMPONENT
#define EMPTY_GRAPHICS_COMPONENT

#include "./component.cpp"
#include "../graphics.cpp"
#include "../game_world.cpp"

class EmptyGraphicsComponent: public GraphicsComponent {
  public:
    EmptyGraphicsComponent(): GraphicsComponent() {};
    virtual void Render(GameObject&, Graphics&) override;
    ~EmptyGraphicsComponent() override {};

  private:

};

void EmptyGraphicsComponent::Render(GameObject& wld, Graphics& graphics) {
  // Do nothing here
}

#endif // EMPTY_GRAPHICS_COMPONENT
