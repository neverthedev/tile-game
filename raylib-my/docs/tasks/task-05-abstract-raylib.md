# TASK-05: Abstract Raylib Dependencies

**Priority:** 🟡 High
**Status:** 📋 TODO
**Estimated Effort:** High
**Last Reviewed:** 2026-01-19

---

## Current Status Assessment

**Relevance:** CRITICAL - This task is essential for:
- Making the codebase testable without graphics initialization
- Enabling unit tests in CI/CD environments
- Improving code maintainability and flexibility
- Following SOLID principles (Dependency Inversion)

**Current State (2026-01-19 - UPDATED):**
- ✅ Custom type wrappers created (Position2D, Rectangle2D, Color2D, GrphCamera)
- ✅ Common types are now raylib-independent
- ✅ Graphics class is completely independent of GameCamera
- ✅ Graphics uses only GrphCamera (platform-independent camera)
- ✅ Camera conversion happens at component boundary (GameCamera → GrphCamera)
- ✅ BeginMode2D() and MouseToWorld2D() use internal camera state
- ⚠️ Raylib still included in 10 files (reduced from widespread usage)
- ❌ No full abstraction layer with interfaces
- ❌ No platform-specific implementations directory

**Files Currently Including raylib.h (PHASE 1 COMPLETE):**

**Core/Platform Layer (Necessary):**
1. `graphics.h` - Uses Image*, Texture2D, Camera2D in interface
2. `graphics.cpp` - Implements raylib conversion functions
3. `world_tiles/tile_terrain_type.h` - Uses Texture2D, Image as member variables
4. `world_tiles/tile.h` - Include guard to prevent conflicts

**Input Components (Need Interface Abstraction):**
5. `input_components/component.cpp` - Direct raylib input calls
6. `input_components/camera_component.cpp` - Uses GetScreenToWorld2D

**Graphics Components (Need Interface Abstraction):**
7. `graphics_components/world_component.h/cpp` - Uses raylib drawing functions
8. `graphics_components/tile_component.cpp` - Uses ImageDraw

**Game Logic (Should Be Eliminated):**
9. `game_interface.cpp` - Uses GetMousePosition(), CheckCollisionPointRec() (temporary)

**Progress:**
- ✅ Phase 1 Complete: Type abstraction (Position2D, Rectangle2D, Color2D, GameCamera)
- ⏳ Phase 2 In Progress: Minimize raylib includes (10 files, down from 15+)
- ❌ Phase 3 Not Started: Interface layer creation
- ❌ Phase 4 Not Started: Platform implementations

**Complexity:** HIGH - Multi-phase systematic refactoring across entire codebase

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

### Phase 1: Create Base Abstractions ✅ COMPLETE

**Goal:** Establish type wrappers without breaking existing code

**Completed:**
1. ✅ Created `Color2D` class - platform-independent color type
2. ✅ Removed raylib includes from `Position2D` - no longer depends on Vector2
3. ✅ Removed raylib includes from `Rectangle2D` - no longer depends on Rectangle
4. ✅ Removed raylib includes from `GameCamera` - uses Position2D instead of Vector2
5. ✅ Created `GrphCamera` class - platform-independent camera (common/grph_camera.h/cpp)
6. ✅ Graphics class completely independent of GameCamera - uses only GrphCamera
7. ✅ Removed all GameCamera parameters from Graphics methods (BeginMode2D, MouseToWorld2D)
8. ✅ Camera conversion at component boundary (CameraComponent converts GameCamera → GrphCamera)
9. ✅ Updated Graphics interface to use custom types (Color2D, Position2D, Rectangle2D, GrphCamera)
10. ✅ All raylib type conversions moved to graphics.cpp implementation
11. ✅ Documentation created (docs/grph_camera_system.md with UML sequence diagrams)

**Results:**
- Graphics.h includes raylib.h (necessary for Image*, Texture2D in interface)
- Common types (Position2D, Rectangle2D, Color2D, GrphCamera) are raylib-independent
- Graphics class has NO dependency on GameCamera (uses GrphCamera internally)
- Camera conversion happens at component layer (GameCamera → GrphCamera → Camera2D)
- BeginMode2D() and MouseToWorld2D() use internal GrphCamera state (no parameters)
- Conversion functions in graphics.cpp handle all type bridging
- Reduced raylib.h includes from 15+ files to 10 files
- Comprehensive documentation with UML sequence diagrams

**Time Taken:** ~3 hours
**Risk:** Low (additive changes, conversion layer works)

### Phase 2: Create Interface Layer ⭐ NEXT STEP

**Goal:** Define abstract interfaces for platform independence

1. **Create interface directory structure:**
   ```bash
   mkdir -p src/interfaces
   mkdir -p src/platform/raylib
   ```

2. **Define core interfaces:**
   - `src/interfaces/i_input_system.h` - Mouse, keyboard input
   - `src/interfaces/i_render_system.h` - Drawing operations
   - `src/interfaces/i_collision_system.h` - Collision detection
   - `src/interfaces/i_resource_system.h` - Texture/image loading

3. **Create Raylib implementations:**
   - `src/platform/raylib/raylib_input_system.h/cpp`
   - `src/platform/raylib/raylib_render_system.h/cpp`
   - `src/platform/raylib/raylib_collision_system.h/cpp`
   - `src/platform/raylib/raylib_resource_system.h/cpp`

**Estimated Time:** 4-6 hours
**Risk:** Low (additive changes only)

### Phase 2: Refactor Graphics Class

**Goal:** Update Graphics to use dependency injection

1. **Update Graphics constructor:**
   ```cpp
   Graphics(std::unique_ptr<IRenderSystem> renderer,
            std::unique_ptr<IInputSystem> input,
            std::unique_ptr<ICollisionSystem> collision,
            std::unique_ptr<IResourceSystem> resources,
            int width, int height, ...);
   ```

2. **Add delegation methods:**
   - Forward input calls to IInputSystem
   - Forward rendering to IRenderSystem
   - Forward collision to ICollisionSystem

3. **Update main.cpp to inject dependencies**

**Estimated Time:** 3-4 hours
**Risk:** Medium (changes core class)

### Phase 3: Remove Direct Raylib Calls

**Goal:** Eliminate all direct raylib dependencies from game logic

1. **Update game_interface.cpp:**
   - Replace `GetMousePosition()` → `graphics.GetMousePosition()`
   - Replace `CheckCollisionPointRec()` → `graphics.CheckCollision()`

2. **Update input_components:**
   - Pass Graphics reference to components
   - Use Graphics methods instead of direct raylib

3. **Remove unnecessary `#include "raylib.h"`:**
   - Keep only in platform-specific files
   - Use forward declarations where possible

**Estimated Time:** 4-5 hours
**Risk:** Medium (multiple file changes)

### Phase 4: Create Test Infrastructure

**Goal:** Enable unit testing with mocks

1. **Create mock implementations:**
   - `test/mocks/mock_input_system.h`
   - `test/mocks/mock_render_system.h`
   - `test/mocks/mock_collision_system.h`

2. **Write example tests:**
   - Test input handling with mock input
   - Test collision detection with mock collision
   - Verify headless execution

**Estimated Time:** 3-4 hours
**Risk:** Low (test-only code)

---

## Clear Action Plan (Updated 2026-01-19)

### Completed Steps:

1. ✅ **DONE:** Assessment of current codebase state
2. ✅ **DONE:** Created `Color2D` class (common/color_2d.h/cpp)
3. ✅ **DONE:** Created `GrphCamera` class (common/grph_camera.h/cpp)
4. ✅ **DONE:** Removed raylib dependencies from Position2D, Rectangle2D, GameCamera
5. ✅ **DONE:** Removed GameCamera dependency from Graphics class
6. ✅ **DONE:** Graphics uses only GrphCamera internally (platform-independent)
7. ✅ **DONE:** Removed camera parameters from BeginMode2D() and MouseToWorld2D()
8. ✅ **DONE:** Camera conversion at component boundary (CameraComponent)
9. ✅ **DONE:** Updated Graphics interface to use custom types only
10. ✅ **DONE:** Minimized raylib includes to 10 files (from 15+)
11. ✅ **DONE:** Build succeeds with new type system
12. ✅ **DONE:** Created documentation with UML sequence diagrams (docs/grph_camera_system.md)

### Immediate Next Steps (Phase 2):

1. **TODO:** Create `src/interfaces/i_input_system.h` with methods:
   - `GetMousePosition() → Position2D`
   - `IsKeyPressed(int key) → bool`
   - `IsMouseButtonPressed(int button) → bool`
   - `IsKeyDown(int key) → bool`
   - `GetMouseWheelMove() → float`

3. **TODO:** Create `src/interfaces/i_collision_system.h` with methods:
   - `CheckCollisionPointRec()`
   - `CheckCollisionRecs()`

4. **TODO:** Create `src/platform/raylib/raylib_input_system.h/cpp`
   - Implement interface by delegating to raylib functions

5. **TODO:** Create `src/platform/raylib/raylib_collision_system.h/cpp`
   - Implement interface by delegating to raylib functions

### Success Criteria:

**Phase 1 (Type Abstraction):**
- [x] Common types (Position2D, Rectangle2D, Color2D, GrphCamera) are raylib-independent
- [x] Graphics interface uses only custom types (no raylib types in public interface)
- [x] Graphics is completely independent of GameCamera (uses GrphCamera internally)
- [x] GameCamera uses Position2D instead of Vector2
- [x] Camera conversion at component boundary (GameCamera → GrphCamera)
- [x] BeginMode2D() and MouseToWorld2D() use internal camera (no parameters)
- [x] Build succeeds
- [x] Raylib includes reduced to essential files only
- [x] Documentation created with UML sequence diagrams

**Phase 2 (Interface Layer):**
- [ ] All interfaces defined (IInputSystem, ICollisionSystem, IRenderSystem, IResourceSystem)
- [ ] All Raylib implementations created
- [ ] Graphics class uses dependency injection
- [ ] Mock implementations for testing created

**Phase 3 (Eliminate Direct Calls):**
- [ ] No direct raylib calls in game logic files
- [ ] Input components use interfaces
- [ ] Graphics components use interfaces
- [ ] Only platform layer includes raylib.h

**Phase 4 (Testing):**
- [ ] At least one unit test runs without graphics init
- [ ] Mock-based tests for input handling
- [ ] Mock-based tests for collision detection
- [ ] CMakeLists.txt updated with test infrastructure

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

---

## Summary for AI Implementation

**What needs to be done:**

This task requires creating an abstraction layer to decouple game logic from the raylib graphics library. The main work involves:

1. **Create interface files** defining abstract methods for input, rendering, collision, and resource management
2. **Implement raylib-specific versions** of these interfaces that wrap existing raylib calls
3. **Refactor the Graphics class** to accept and use these interfaces via dependency injection
4. **Update all game logic files** to call methods through the Graphics class instead of directly calling raylib
5. **Create mock implementations** for testing purposes

**Why it's needed:**

Currently, game logic is tightly coupled to raylib, making it impossible to:
- Write unit tests without initializing graphics
- Run tests in headless CI/CD environments
- Mock input/rendering for automated testing
- Port to other rendering libraries

**Current obstacles:**

- 8+ files have direct raylib dependencies
- No abstraction layer exists yet
- Graphics class provides some wrapping but inconsistently
- Common types (Position2D, Rectangle2D) include raylib.h

**Is it clear?**

YES - The task is well-defined with:
- ✅ Clear problem statement
- ✅ Detailed interface designs
- ✅ Step-by-step implementation plan
- ✅ Code examples for each phase
- ✅ Success criteria
- ✅ Files affected identified
- ✅ Testing strategy outlined

**Ready to implement:** YES - All information needed to start work is available.
