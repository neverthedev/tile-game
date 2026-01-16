#ifndef INPUT_COMPONENT_H
#define INPUT_COMPONENT_H

#include "../common/game_object.cpp"
#include "../common/position_2d.cpp"

class InputComponent {
  public:
    InputComponent();
    virtual void HandleInput(GameObject&) = 0;
    virtual ~InputComponent();
  protected:
    bool IsKeyDown(KeyboardKey);
    Position2D GetMousePosition();
    float GetMouseWheelMove();
    float Clamp(float, float, float);
};

#endif // INPUT_COMPONENT_H
