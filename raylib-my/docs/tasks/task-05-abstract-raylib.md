# TASK-05: Abstract Raylib Dependencies

**Priority:** 🟡 High
**Status:** 📋 TODO
**Estimated Effort:** High

---

## Problem Description

The codebase has tight coupling to raylib throughout, making it difficult to:
- Unit test game logic without initializing graphics
- Port to other rendering libraries
- Test in headless environments
- Mock graphics for testing

Current direct dependencies on raylib:
```cpp
#include "raylib.h"

Vector2 mouse = GetMousePosition();
if (CheckCollisionPointRec(mouse, rect)) { /* ... */ }
```

### Why This Is Wrong

1. **Tight Coupling**: Game logic directly depends on rendering library
2. **Hard to Test**: Can't unit test without initializing raylib window
3. **Not Portable**: Changing graphics library requires rewriting code
4. **No Mocking**: Can't mock input/rendering for tests
5. **Violates Dependency Inversion**: High-level modules depend on low-level details

### Current Impact

- No unit tests for game logic
- Can't test headlessly (CI/CD challenges)
- Graphics initialization required for all tests
- Difficult to add replay system or AI players
- Can't easily support multiple renderers

---

## Desired Outcome

Create abstraction layers that decouple game logic from raylib specifics.

### Target Architecture

```
Game Logic → Interfaces → Raylib Implementation
                ↓
             Mock/Test Implementation
```

### Interface Design

```cpp
// interfaces/i_input_system.h
class IInputSystem {
  public:
    virtual ~IInputSystem() = default;

    virtual Vector2 GetMousePosition() const = 0;
    virtual bool IsKeyPressed(int key) const = 0;
    virtual bool IsMouseButtonPressed(int button) const = 0;
    virtual bool IsKeyDown(int key) const = 0;
};

// interfaces/i_render_system.h
class IRenderSystem {
  public:
    virtual ~IRenderSystem() = default;

    virtual void BeginDrawing() = 0;
    virtual void EndDrawing() = 0;
    virtual void ClearBackground(Color color) = 0;
    virtual void DrawTexture(Texture2D texture, int x, int y, Color tint) = 0;
    virtual void DrawRectangle(int x, int y, int w, int h, Color color) = 0;
    // ... other drawing methods
};

// interfaces/i_collision_system.h
class ICollisionSystem {
  public:
    virtual ~ICollisionSystem() = default;

    virtual bool CheckCollisionPointRec(Vector2 point, Rectangle rect) const = 0;
    virtual bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2) const = 0;
};

// interfaces/i_resource_system.h
class IResourceSystem {
  public:
    virtual ~IResourceSystem() = default;

    virtual Texture2D LoadTexture(const char* filename) = 0;
    virtual Image LoadImage(const char* filename) = 0;
    virtual void UnloadTexture(Texture2D texture) = 0;
};
```

### Raylib Implementation

```cpp
// platform/raylib_input_system.h
class RaylibInputSystem : public IInputSystem {
  public:
    Vector2 GetMousePosition() const override {
      return ::GetMousePosition();
    }

    bool IsKeyPressed(int key) const override {
      return ::IsKeyPressed(key);
    }

    bool IsMouseButtonPressed(int button) const override {
      return ::IsMouseButtonPressed(button);
    }

    bool IsKeyDown(int key) const override {
      return ::IsKeyDown(key);
    }
};

// platform/raylib_render_system.h
class RaylibRenderSystem : public IRenderSystem {
  public:
    void BeginDrawing() override {
      ::BeginDrawing();
    }

    void EndDrawing() override {
      ::EndDrawing();
    }

    void ClearBackground(Color color) override {
      ::ClearBackground(color);
    }

    // ... implement other methods
};
```

### Mock Implementation for Testing

```cpp
// test/mock_input_system.h
class MockInputSystem : public IInputSystem {
  public:
    void SetMousePosition(Vector2 pos) { mousePos = pos; }
    void SetKeyPressed(int key, bool pressed) { keysPressed[key] = pressed; }

    Vector2 GetMousePosition() const override { return mousePos; }
    bool IsKeyPressed(int key) const override {
      return keysPressed.count(key) && keysPressed.at(key);
    }

  private:
    Vector2 mousePos{0, 0};
    std::map<int, bool> keysPressed;
};
```

---

## Implementation Steps

### 1. Define Interface Layer

Create `src/interfaces/` directory with:
- `i_input_system.h`
- `i_render_system.h`
- `i_collision_system.h`
- `i_resource_system.h`
- `i_window_system.h`

### 2. Create Raylib Implementations

Create `src/platform/raylib/` directory with implementations:
- `raylib_input_system.h/cpp`
- `raylib_render_system.h/cpp`
- `raylib_collision_system.h/cpp`
- `raylib_resource_system.h/cpp`
- `raylib_window_system.h/cpp`

### 3. Update Graphics Class

```cpp
class Graphics {
  public:
    Graphics(std::unique_ptr<IRenderSystem> renderer,
             std::unique_ptr<IInputSystem> input,
             std::unique_ptr<ICollisionSystem> collision,
             std::unique_ptr<IResourceSystem> resources)
      : renderer_(std::move(renderer)),
        input_(std::move(input)),
        collision_(std::move(collision)),
        resources_(std::move(resources)) {}

    void BeginDrawing() { renderer_->BeginDrawing(); }
    void EndDrawing() { renderer_->EndDrawing(); }
    Vector2 GetMousePosition() const { return input_->GetMousePosition(); }

  private:
    std::unique_ptr<IRenderSystem> renderer_;
    std::unique_ptr<IInputSystem> input_;
    std::unique_ptr<ICollisionSystem> collision_;
    std::unique_ptr<IResourceSystem> resources_;
};
```

### 4. Update Main Application

```cpp
int main() {
  // Create raylib implementations
  auto renderer = std::make_unique<RaylibRenderSystem>();
  auto input = std::make_unique<RaylibInputSystem>();
  auto collision = std::make_unique<RaylibCollisionSystem>();
  auto resources = std::make_unique<RaylibResourceSystem>();

  // Inject into Graphics
  Graphics graphics(std::move(renderer),
                   std::move(input),
                   std::move(collision),
                   std::move(resources));

  // Rest of game initialization...
}
```

### 5. Update Game Logic

Remove direct raylib calls:

```cpp
// Before
Vector2 mouse = GetMousePosition();
if (CheckCollisionPointRec(mouse, rect)) { /* ... */ }

// After
Vector2 mouse = graphics.GetMousePosition();
if (graphics.CheckCollisionPointRec(mouse, rect)) { /* ... */ }
```

---

## Files Affected

- `src/graphics.cpp` - Major refactoring
- `src/game_interface_.cpp` - Remove direct raylib calls
- `src/game_world.cpp` - Remove direct raylib calls
- `src/game_camera.cpp` - Use interfaces instead
- All files with `#include "raylib.h"`

---

## Testing Benefits

### Unit Test Example

```cpp
TEST(GameWorld, HandlesMouseClick) {
  // Create mock systems
  auto mockInput = std::make_unique<MockInputSystem>();
  mockInput->SetMousePosition({100, 100});
  mockInput->SetMouseButtonPressed(MOUSE_LEFT_BUTTON, true);

  auto mockRenderer = std::make_unique<MockRenderSystem>();

  // Create testable graphics
  Graphics graphics(std::move(mockRenderer), std::move(mockInput), ...);

  // Test game logic without real graphics
  GameWorld world(...);
  world.HandleInput();

  // Verify behavior
  EXPECT_TRUE(world.IsTileSelected({5, 5}));
}
```

### Headless Testing

```cpp
// CI/CD environment - no display needed
auto nullRenderer = std::make_unique<NullRenderSystem>();  // No-op rendering
auto replayInput = std::make_unique<ReplayInputSystem>(recordedInputs);

Graphics graphics(std::move(nullRenderer), std::move(replayInput), ...);
// Run game headlessly for testing
```

---

## Benefits After Completion

✅ Game logic testable independently
✅ Can mock input/rendering
✅ Headless testing possible
✅ Support multiple rendering backends
✅ Better separation of concerns
✅ Follows Dependency Inversion Principle
✅ Enables replay systems
✅ Portable to other platforms

---

## Alternative Approaches

### Option 1: Adapter Pattern (Lighter weight)

```cpp
class RaylibAdapter {
  public:
    static Vector2 GetMousePosition() { return ::GetMousePosition(); }
    static bool IsKeyPressed(int key) { return ::IsKeyPressed(key); }
};

// For testing, provide MockAdapter
class MockAdapter {
  public:
    static Vector2 GetMousePosition() { return mockMousePos; }
    static bool IsKeyPressed(int key) { return mockKeys[key]; }
    static Vector2 mockMousePos;
    static std::map<int, bool> mockKeys;
};

// Use template for compile-time selection
template<typename PlatformAdapter = RaylibAdapter>
class Graphics {
  Vector2 GetMousePosition() {
    return PlatformAdapter::GetMousePosition();
  }
};
```

### Option 2: Preprocessor Macros (Not recommended)

```cpp
#ifdef TESTING
  #define PLATFORM_GET_MOUSE_POSITION() MockGetMousePosition()
#else
  #define PLATFORM_GET_MOUSE_POSITION() GetMousePosition()
#endif
```

---

## Migration Strategy

### Phase 1: Create Interfaces (Non-breaking)
- Define interface classes
- Create raylib implementations
- No changes to existing code yet

### Phase 2: Inject Dependencies (Gradual)
- Update Graphics to accept interfaces
- Keep backward compatibility
- Default to raylib implementations

### Phase 3: Remove Direct Calls (Breaking)
- Replace all `GetMousePosition()` with `graphics.GetMousePosition()`
- Remove `#include "raylib.h"` from non-platform files
- Update all call sites

### Phase 4: Add Tests
- Create mock implementations
- Write unit tests
- Verify all functionality

---

## Performance Considerations

**Virtual function overhead:**
- Minimal (<1% typically)
- Optimized away with LTO
- Worth it for testability

**Alternative if critical:**
- Template-based (no virtual calls)
- Compile-time polymorphism
- Zero-cost abstraction

---

## References

- [Dependency Inversion Principle](https://en.wikipedia.org/wiki/Dependency_inversion_principle)
- [C++ Core Guidelines - I.4](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Ri-abstract)
- [Game Programming Patterns - Service Locator](https://gameprogrammingpatterns.com/service-locator.html)
