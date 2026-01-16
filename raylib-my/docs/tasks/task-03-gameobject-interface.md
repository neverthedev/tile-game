# TASK-03: Split GameObject Interface

**Priority:** 🟡 High
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

The current `GameObject` interface forces all entities to implement three methods, even if they don't need all of them:

```cpp
class GameObject {
  public:
    virtual void HandleInput() = 0;
    virtual void Update() = 0;
    virtual void Render(Graphics&) = 0;
    virtual ~GameObject();
};
```

### Why This Is Wrong

1. **Violates Interface Segregation Principle (ISP)**: Clients shouldn't depend on interfaces they don't use
2. **Forces Unnecessary Implementations**: Entities that don't need input must implement empty `HandleInput()`
3. **Rigid Design**: Can't compose objects with only some capabilities
4. **Poor Flexibility**: Adding new capabilities requires changing base interface
5. **Hidden Contracts**: Empty implementations hide what object actually does

### Current Impact

Example problematic scenarios:
- Static decorations must implement `HandleInput()` even though they never handle input
- Pure UI elements might not need `Update()`
- Data-only objects forced to provide rendering logic

---

## Desired Outcome

Split into smaller, focused interfaces following the Interface Segregation Principle:

### Option 1: Separate Interfaces

```cpp
// Base interface for all game entities
class IGameEntity {
  public:
    virtual ~IGameEntity() = default;
};

// Optional capabilities
class IInputHandler {
  public:
    virtual void HandleInput() = 0;
    virtual ~IInputHandler() = default;
};

class IUpdatable {
  public:
    virtual void Update() = 0;
    virtual ~IUpdatable() = default;
};

class IRenderable {
  public:
    virtual void Render(Graphics& graphics) = 0;
    virtual ~IRenderable() = default;
};

// Compose as needed
class GameWorld : public IGameEntity,
                  public IInputHandler,
                  public IUpdatable,
                  public IRenderable {
  public:
    void HandleInput() override { /* ... */ }
    void Update() override { /* ... */ }
    void Render(Graphics& graphics) override { /* ... */ }
};

// Static decoration - only renderable
class Decoration : public IGameEntity,
                   public IRenderable {
  public:
    void Render(Graphics& graphics) override { /* ... */ }
    // No HandleInput or Update needed!
};
```

### Option 2: Component-Based (More flexible)

```cpp
class GameObject {
  public:
    void AddComponent(std::unique_ptr<IComponent> component);

    template<typename T>
    T* GetComponent();

    void HandleInput() {
      for (auto& component : components) {
        if (auto* inputComp = dynamic_cast<IInputHandler*>(component.get())) {
          inputComp->HandleInput();
        }
      }
    }

  private:
    std::vector<std::unique_ptr<IComponent>> components;
};

// Components implement only what they need
class CameraInputComponent : public IInputHandler {
  void HandleInput() override { /* handle camera input */ }
};

class WorldRenderComponent : public IRenderable {
  void Render(Graphics& graphics) override { /* render world */ }
};
```

---

## Implementation Steps

### 1. Define New Interfaces

Create `src/common/game_entity_interfaces.h`:
```cpp
#ifndef GAME_ENTITY_INTERFACES_H
#define GAME_ENTITY_INTERFACES_H

class IGameEntity {
  public:
    virtual ~IGameEntity() = default;
};

class IInputHandler {
  public:
    virtual void HandleInput() = 0;
    virtual ~IInputHandler() = default;
};

class IUpdatable {
  public:
    virtual void Update() = 0;
    virtual ~IUpdatable() = default;
};

class IRenderable {
  public:
    virtual void Render(Graphics& graphics) = 0;
    virtual ~IRenderable() = default;
};

#endif
```

### 2. Update Existing Classes

Update each class to implement only needed interfaces:

```cpp
// GameWorld needs all three
class GameWorld : public IGameEntity,
                  public IInputHandler,
                  public IUpdatable,
                  public IRenderable {
  // ...
};

// GameCamera needs all three
class GameCamera : public IGameEntity,
                   public IInputHandler,
                   public IUpdatable,
                   public IRenderable {
  // ...
};

// Static menu might only need render and update
class Menu : public IGameEntity,
             public IUpdatable,
             public IRenderable {
  // No HandleInput if not interactive
};
```

### 3. Update GameInterface

Modify `GameInterface` to work with separate interfaces:

```cpp
void GameInterface::HandleInput() {
  for (auto& area : gameAreas) {
    if (auto* inputHandler = dynamic_cast<IInputHandler*>(area.get())) {
      inputHandler->HandleInput();
    }
  }
}

void GameInterface::Update() {
  for (auto& area : gameAreas) {
    if (auto* updatable = dynamic_cast<IUpdatable*>(area.get())) {
      updatable->Update();
    }
  }
}

void GameInterface::Render(Graphics& graphics) {
  for (auto& area : gameAreas) {
    if (auto* renderable = dynamic_cast<IRenderable*>(area.get())) {
      renderable->Render(graphics);
    }
  }
}
```

### 4. Update GameArea

```cpp
class GameArea {
  public:
    GameArea(IGameEntity& obj, Rectangle2D pos, int priority)
      : entity(obj), position(pos), priority(priority) {}

    void HandleInput() {
      if (auto* handler = dynamic_cast<IInputHandler*>(&entity)) {
        handler->HandleInput();
      }
    }

    void Update() {
      if (auto* updatable = dynamic_cast<IUpdatable*>(&entity)) {
        updatable->Update();
      }
    }

    void Render(Graphics& graphics) {
      if (auto* renderable = dynamic_cast<IRenderable*>(&entity)) {
        renderable->Render(graphics);
      }
    }

  private:
    IGameEntity& entity;
    Rectangle2D position;
    int priority;
};
```

---

## Files Affected

- `src/common/game_object.cpp` → Split into separate interface files
- `src/game_interface_.cpp` → Update to use dynamic_cast
- `src/game_world.cpp` → Implement only needed interfaces
- `src/game_camera.cpp` → Implement only needed interfaces
- `src/menus/menu.cpp` → Implement only needed interfaces
- All other classes inheriting from GameObject

---

## Alternative: Template-Based Approach

For compile-time polymorphism (no virtual functions):

```cpp
template<typename T>
void HandleInputIfPossible(T& obj) {
  if constexpr (requires { obj.HandleInput(); }) {
    obj.HandleInput();
  }
}

// Usage
for (auto& area : gameAreas) {
  HandleInputIfPossible(*area);
}
```

---

## Benefits After Completion

✅ Follows Interface Segregation Principle
✅ More flexible object composition
✅ Clearer which capabilities objects have
✅ No empty method implementations
✅ Easier to add new capabilities
✅ Better testability
✅ Self-documenting code

---

## Testing Approach

1. Create new interfaces
2. Update one class at a time
3. Verify compilation
4. Test that objects still function correctly
5. Verify no runtime errors from dynamic_cast

### Test Cases

- Object with all interfaces works
- Object with only render interface works
- Object with no interfaces doesn't crash
- GameInterface correctly identifies capabilities

---

## Performance Considerations

**dynamic_cast overhead:**
- Small overhead for type checking
- Can be eliminated with template approach
- Usually negligible for game loop

**Alternatives if performance critical:**
- Use template metaprogramming (compile-time)
- Use bitflags for capability checking
- Use type erasure with function pointers

---

## Migration Strategy

### Phase 1: Add new interfaces (non-breaking)
- Create new interface files
- Keep GameObject as-is
- Classes can implement both old and new

### Phase 2: Update classes (gradual)
- Update one class at a time
- Test after each change
- Ensure backward compatibility

### Phase 3: Remove GameObject (breaking)
- After all classes updated
- Remove old GameObject interface
- Clean up any remaining dependencies

---

## References

- [SOLID Principles - ISP](https://en.wikipedia.org/wiki/Interface_segregation_principle)
- [C++ Core Guidelines - I.4](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
- [Effective C++ Item 31: Make functions non-virtual when possible](https://www.aristeia.com/books.html)
