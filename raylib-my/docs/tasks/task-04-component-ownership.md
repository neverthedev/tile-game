# TASK-04: Clarify Component Ownership

**Priority:** 🟡 High
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

Component ownership is unclear throughout the codebase. Components are passed by reference but lifetime management is ambiguous:

```cpp
class GameCamera: public GameObject {
  public:
    GameCamera(InputComponent &c, GraphicsComponent& g);
  private:
    InputComponent& input;
    GraphicsComponent& render;
};

GameWorld::GameWorld(/* ... */, InputComponent& c_inp, GraphicsComponent& c_rnd):
  input { c_inp },
  render { c_rnd }
{
  // ...
}
```

### Why This Is Wrong

1. **Unclear Lifetime**: Who creates these components? Who destroys them?
2. **Dangling References**: References can become invalid if component deleted
3. **No Ownership Semantics**: Can't tell if components are shared or unique
4. **Memory Management Unclear**: Where is `new`/`delete` happening?
5. **Difficult to Reason About**: Must trace through entire codebase to understand lifetime

### Current Impact

Questions that are hard to answer:
- Who owns the InputComponent?
- Can components outlive the GameCamera?
- Can multiple objects share the same component?
- When are components destroyed?
- What happens if component is deleted while referenced?

---

## Desired Outcome

Make ownership explicit and safe using modern C++ idioms.

### Option 1: Unique Ownership (Recommended if not shared)

```cpp
class GameCamera: public GameObject {
  public:
    GameCamera(std::unique_ptr<InputComponent> inputComp,
               std::unique_ptr<GraphicsComponent> graphicsComp)
      : input(std::move(inputComp)),
        render(std::move(graphicsComp)) {}

  private:
    std::unique_ptr<InputComponent> input;
    std::unique_ptr<GraphicsComponent> render;
};

// Usage
auto inputComp = std::make_unique<CameraInputComponent>();
auto graphicsComp = std::make_unique<CameraGraphicsComponent>();
auto camera = std::make_unique<GameCamera>(std::move(inputComp),
                                           std::move(graphicsComp));
```

**Clear semantics:**
- GameCamera owns the components
- Components destroyed when GameCamera destroyed
- Cannot be shared with other objects
- Lifetime guaranteed

### Option 2: Shared Ownership (If components can be shared)

```cpp
class GameCamera: public GameObject {
  public:
    GameCamera(std::shared_ptr<InputComponent> inputComp,
               std::shared_ptr<GraphicsComponent> graphicsComp)
      : input(inputComp),
        render(graphicsComp) {}

  private:
    std::shared_ptr<InputComponent> input;
    std::shared_ptr<GraphicsComponent> render;
};

// Usage - can share components
auto inputComp = std::make_shared<CameraInputComponent>();
auto graphicsComp = std::make_shared<CameraGraphicsComponent>();

auto camera1 = std::make_unique<GameCamera>(inputComp, graphicsComp);
auto camera2 = std::make_unique<GameCamera>(inputComp, graphicsComp);  // Shares input!
```

**Clear semantics:**
- Components are reference-counted
- Destroyed when last owner is destroyed
- Can be safely shared
- Lifetime managed automatically

### Option 3: Non-Owning References (If components owned elsewhere)

```cpp
class GameCamera: public GameObject {
  public:
    GameCamera(InputComponent& inputComp,
               GraphicsComponent& graphicsComp)
      : input(inputComp),
        render(graphicsComp) {}

  private:
    InputComponent& input;         // Non-owning reference
    GraphicsComponent& render;     // Non-owning reference

    // OR use raw pointers to allow null
    InputComponent* input;         // Non-owning, nullable
    GraphicsComponent* render;     // Non-owning, nullable
};

// Usage - components owned by component manager
ComponentManager manager;
manager.CreateInputComponent<CameraInputComponent>("camera_input");
manager.CreateGraphicsComponent<CameraGraphicsComponent>("camera_graphics");

auto camera = std::make_unique<GameCamera>(
  manager.GetInputComponent("camera_input"),
  manager.GetGraphicsComponent("camera_graphics")
);
```

**Clear semantics:**
- GameCamera does NOT own components
- Components must outlive GameCamera
- Managed by external system
- Document lifetime requirements clearly

---

## Implementation Steps

### 1. Analyze Component Lifetime

For each component usage, determine:
- Is the component created just for this object?
- Can it be shared across multiple objects?
- Who should manage the lifetime?
- Should there be a central component manager?

### 2. Choose Ownership Model

Based on analysis:

| Scenario | Use |
|----------|-----|
| Component unique to object | `unique_ptr` |
| Component shared across objects | `shared_ptr` |
| Component managed externally | Reference or raw pointer |
| Component may not exist | `shared_ptr` or raw pointer (nullable) |

### 3. Update Component Storage

Example for GameCamera with unique ownership:

```cpp
// game_camera.h
class GameCamera: public GameObject {
  public:
    GameCamera(std::unique_ptr<InputComponent> inputComp,
               std::unique_ptr<GraphicsComponent> graphicsComp);

  private:
    std::unique_ptr<InputComponent> input;
    std::unique_ptr<GraphicsComponent> render;
};

// game_camera.cpp
GameCamera::GameCamera(std::unique_ptr<InputComponent> inputComp,
                       std::unique_ptr<GraphicsComponent> graphicsComp)
  : input(std::move(inputComp)),
    render(std::move(graphicsComp)) {
  // Components now owned by GameCamera
}

void GameCamera::HandleInput() {
  input->HandleInput();  // Use like normal pointer
}
```

### 4. Update Creation Sites

```cpp
// Before (unclear ownership)
InputComponent& camInput = /* ??? where does this come from? */
GraphicsComponent& camRender = /* ??? */
GameCamera camera(camInput, camRender);

// After (clear ownership)
auto camera = std::make_unique<GameCamera>(
  std::make_unique<CameraInputComponent>(),
  std::make_unique<CameraGraphicsComponent>()
);
```

### 5. Document Ownership

Add comments for non-owning references:

```cpp
class GameWorld {
  public:
    // Non-owning reference. TilesManager must outlive GameWorld.
    GameWorld(const TilesManager& tilesManager)
      : tilesManager(tilesManager) {}

  private:
    const TilesManager& tilesManager;  // Non-owning
};
```

---

## Files Affected

- `src/game_camera.cpp` - Component ownership
- `src/game_world.cpp` - Component ownership, TilesManager reference
- `src/menus/decoration_menu.cpp` - Component ownership
- All component creation sites
- Component factory methods

---

## Design Patterns to Consider

### Component Manager Pattern

```cpp
class ComponentManager {
  public:
    template<typename T, typename... Args>
    std::shared_ptr<T> Create(Args&&... args) {
      auto component = std::make_shared<T>(std::forward<Args>(args)...);
      components.push_back(component);
      return component;
    }

    void UpdateAll() {
      for (auto& comp : components) {
        if (auto updatable = std::dynamic_pointer_cast<IUpdatable>(comp)) {
          updatable->Update();
        }
      }
    }

  private:
    std::vector<std::shared_ptr<IComponent>> components;
};
```

### Factory with Ownership Transfer

```cpp
class ComponentFactory {
  public:
    static std::unique_ptr<InputComponent> CreateCameraInput() {
      return std::make_unique<CameraInputComponent>();
    }

    static std::unique_ptr<GraphicsComponent> CreateCameraGraphics() {
      return std::make_unique<CameraGraphicsComponent>();
    }
};

// Usage
auto camera = std::make_unique<GameCamera>(
  ComponentFactory::CreateCameraInput(),
  ComponentFactory::CreateCameraGraphics()
);
```

---

## Benefits After Completion

✅ Clear ownership semantics
✅ No dangling references
✅ Automatic lifetime management
✅ Self-documenting code
✅ Easier to reason about
✅ Prevents memory bugs
✅ Follows C++ best practices

---

## Testing Approach

1. Update one class at a time
2. Ensure all creation sites updated
3. Test object destruction (use debugger or prints)
4. Verify no crashes or leaks
5. Document any non-owning references

### Test Cases

- Create and destroy object - components cleaned up
- Multiple objects sharing components (if using shared_ptr)
- Component outlives object (if non-owning reference)

---

## Common Patterns

### Pattern 1: Aggregate Ownership
```cpp
class GameObject {
  std::unique_ptr<InputComponent> input;
  std::unique_ptr<GraphicsComponent> graphics;
  std::unique_ptr<UpdateComponent> update;
};
```

### Pattern 2: Shared Components
```cpp
class GameWorld {
  std::shared_ptr<TilesManager> tilesManager;  // Shared
  std::unique_ptr<GameCamera> camera;          // Owned
};
```

### Pattern 3: External Management
```cpp
class Entity {
  IComponent* component;  // Non-owning, managed by ECS system
};
```

---

## References

- [C++ Core Guidelines - R.20](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-owner)
- [C++ Core Guidelines - F.7](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-smart)
- [Smart Pointers in C++](https://en.cppreference.com/w/cpp/memory)
