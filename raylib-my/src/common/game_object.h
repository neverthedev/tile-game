#pragma once

#include <memory>

// Forward declarations
class Graphics;
class InputComponent;
class GraphicsComponent;
class UpdateComponent;

class GameObject {
public:
  GameObject();
  GameObject(GameObject&&) noexcept = default;
  GameObject& operator=(GameObject&&) noexcept = default;

  virtual void HandleInput();
  virtual void Update();
  virtual void Render(Graphics&);
  virtual ~GameObject();

protected:
  GameObject(
    std::unique_ptr<InputComponent> inp,
    std::unique_ptr<GraphicsComponent> grph,
    std::unique_ptr<UpdateComponent> upd = nullptr
  );

  std::unique_ptr<InputComponent> inputComponent;
  std::unique_ptr<GraphicsComponent> graphicsComponent;
  std::unique_ptr<UpdateComponent> updateComponent;
};
