#pragma once

#include <memory>

// Forward declarations
class InputSystem;
class CollisionSystem;
class RenderSystem;
class InputComponent;
class GraphicsComponent;
class UpdateComponent;

class GameObject {
public:
  GameObject();
  GameObject(GameObject&&) noexcept = default;
  GameObject& operator=(GameObject&&) noexcept = default;

  virtual void HandleInput(InputSystem&, CollisionSystem&);
  virtual void Update(CollisionSystem&);
  virtual void Render(RenderSystem&);
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
