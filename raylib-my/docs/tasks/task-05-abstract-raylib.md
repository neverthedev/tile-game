# TASK-05: Abstract Raylib Dependencies

**Priority:** 🟡 High
**Status:** ✅ Complete (Testing infrastructure deferred)
**Estimated Effort:** High
**Last Reviewed:** 2026-01-19
**Completed:** 2026-01-19

---

## Current Status Assessment

**Relevance:** CRITICAL - This task is essential for:
- Making the codebase testable without graphics initialization
- Enabling unit tests in CI/CD environments
- Improving code maintainability and flexibility
- Following SOLID principles (Dependency Inversion)

**Current State (2026-01-19 - COMPLETE):**
- ✅ Custom type wrappers created (Position2D, Rectangle2D, Color2D, GrphCamera)
- ✅ Common types are now raylib-independent
- ✅ Graphics class is completely independent of GameCamera
- ✅ Graphics uses only GrphCamera (platform-independent camera)
- ✅ Camera conversion happens at component boundary (GameCamera → GrphCamera)
- ✅ BeginMode2D() and MouseToWorld2D() use internal camera state
- ✅ ResourcesSystem interface complete (uses TextureHandle/ImageHandle)
- ✅ InputSystem interface complete (GetMousePosition, IsKeyPressed, etc.)
- ✅ CollisionSystem interface complete (CheckCollisionPointRec, etc.)
- ✅ RenderSystem interface complete (BeginDrawing, DrawRectangle, ClearBackground, DrawTexture, DrawFPS, etc.)
- ✅ Graphics implements all 4 interfaces (InputSystem, CollisionSystem, RenderSystem, ResourcesSystem)
- ✅ Raylib isolated to only 2 files (raylib_graphics.h/cpp)
- ✅ GameObject/Component method signatures updated to use interface parameters
- ✅ All components refactored to use interface methods
- ✅ WorldComponent fully refactored (no more direct raylib calls)
- ✅ Tile classes use Rectangle2D instead of raylib Rectangle
- ⏭️ Testing infrastructure deferred to future task

**Files Currently Including raylib.h (FINAL STATE):**

**Core/Platform Layer (Expected):**
1. `graphics/raylib_graphics.h` - Implementation layer header
2. `graphics/raylib_graphics.cpp` - Implementation layer source

**All other files eliminated:**
- ✅ `world_tiles/tile_terrain_type.h` - Now uses Rectangle2D
- ✅ `world_tiles/tile.h` - No longer includes raylib.h
- ✅ `input_components/*.cpp` - Use interface methods
- ✅ `graphics_components/world_component.h/cpp` - Use RenderSystem interface
- ✅ `graphics_components/tile_component.cpp` - Uses interface methods
- ✅ `game_interface.cpp` - Uses interface methods

**Result:** Raylib completely isolated to implementation layer

**Progress:**
- ✅ Phase 1 Complete: Type abstraction (Position2D, Rectangle2D, Color2D, GrphCamera)
- ✅ Phase 2 Complete: Interface layer creation (all 4 interfaces defined and implemented)
  - ✅ InputSystem: GetMousePosition, IsKeyPressed, IsMouseButtonPressed, IsKeyDown, GetMouseWheelMove
  - ✅ CollisionSystem: CheckCollisionPointRec, CheckCollisionRecs
  - ✅ RenderSystem: BeginDrawing, EndDrawing, BeginMode2D, EndMode2D, DrawRectangle, DrawDiamondFrame, ClearBackground, DrawTexture, DrawFPS, GridToScreen, ScreenToWorld2D, MouseToWorld2D, UpdateGrphCamera, GetGrphCamera, GetTileWidth/Height, GetCorrection/SetCorrection, GetDst/SetDst, ImageDraw, GetImageWidth/Height
  - ✅ ResourcesSystem: LoadTexture/Image, UnloadTexture/Image, LoadTextureFromImage, ImageCrop, ImageDrawLineEx, GenImageColor, GetImageWidth/Height
  - ✅ Graphics class implements all 4 interfaces
- ✅ Phase 3 Complete: Updated all call sites to use interface parameters
  - ✅ GameObject base class updated (HandleInput, Update, Render take interface parameters)
  - ✅ Component base classes updated (InputComponent, UpdateComponent, GraphicsComponent)
  - ✅ GameInterface, GameArea updated
  - ✅ All 13+ component implementations updated
  - ✅ Main game loop passes interface references
  - ✅ Direct raylib calls removed from all components
  - ✅ WorldComponent fully refactored (uses only RenderSystem interface)
  - ✅ Tile classes converted from raylib Rectangle to Rectangle2D
  - ✅ Build succeeds with zero raylib dependencies outside implementation layer
- ⏭️ Phase 4 Deferred: Testing infrastructure (mock implementations)
  - Note: Deferred to separate task - core abstraction is complete
  - Future work: Create MockInputSystem, MockRenderSystem, etc.
  - Future work: Add unit tests demonstrating testability

**Current Approach (Updated 2026-01-19):**
- **NO constructor dependency injection** - Keep RaylibGraphics constructor simple
- **Interface parameters** - Pass interfaces as method parameters where needed
- **Single implementation** - RaylibGraphics implements all 4 interfaces (InputSystem, ResourcesSystem, CollisionSystem, RenderSystem)
- **Method signatures updated**:
  - `LoadResources(ResourcesSystem&)`
  - `HandleInput(InputSystem&, CollisionSystem&)`
  - `Update(CollisionSystem&)`
  - `Render(RenderSystem&)`

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

**Time Taken:** ~4 hours
**Risk:** Low (additive changes, conversion layer works)

---

### Phase 2: Create Interface Layer ✅ COMPLETE

**Goal:** Define abstract interfaces for platform independence (NO dependency injection in constructor)

**Completed:**
1. ✅ Created `InputSystem` interface (graphics/input_system.h)
   - GetMousePosition() → Position2D
   - IsKeyPressed(int) → bool
   - IsMouseButtonPressed(int) → bool
   - IsKeyDown(int) → bool
   - GetMouseWheelMove() → float

2. ✅ Created `CollisionSystem` interface (graphics/collision_system.h)
   - CheckCollisionPointRec(Position2D, Rectangle2D) → bool
   - CheckCollisionRecs(Rectangle2D, Rectangle2D) → bool

3. ✅ Created `RenderSystem` interface (graphics/render_system.h)
   - BeginDrawing(), EndDrawing()
   - BeginMode2D(), EndMode2D()
   - DrawRectangle(Rectangle2D, Color2D)
   - DrawDiamondFrame(Position2D, Color2D, bool, float)

4. ✅ Created `ResourcesSystem` interface (graphics/resources_system.h)
   - LoadTexture/Image → TextureHandle/ImageHandle
   - UnloadTexture/Image
   - ImageCrop, ImageDraw, ImageDrawLineEx
   - GenImageColor
   - GetImageWidth/Height

5. ✅ Updated Graphics class
   - Now inherits from all 4 interfaces: InputSystem, CollisionSystem, RenderSystem, ResourcesSystem
   - Implements all interface methods
   - All implementations use raylib internally but expose only custom types

**Results:**
- All interfaces use ONLY custom types (Position2D, Rectangle2D, Color2D, TextureHandle, ImageHandle)
- NO raylib types in any interface (no Vector2, Rectangle, Color, Texture2D, Image)
- Graphics class successfully implements all interfaces
- Build succeeds with all changes
- Ready for Phase 3 (updating call sites to use interface parameters)

**Time Taken:** ~2 hours
**Risk:** Low (interfaces defined, single implementation working)

---

### Phase 2: Create Interface Layer ⭐ IN PROGRESS

---

### Phase 3: Update All Call Sites ⭐ NEXT STEP

**Goal:** Update all GameObject descendants and components to use interface parameters

**1. Update Component Base Classes:**

- `InputComponent::HandleInput(GameObject&, InputSystem&, CollisionSystem&)`
- `UpdateComponent::Update(GameObject&, CollisionSystem&)`
- `GraphicsComponent::Render(GameObject&, RenderSystem&)`

**2. Update All Input Component Implementations:**
- `input_components/camera_component.cpp`
- `input_components/world_component.cpp`
- `input_components/tile_component.cpp`
- `input_components/menu/*.cpp`

**3. Update All Update Component Implementations:**
- `update_components/camera_component.cpp`
- `update_components/world_component.cpp`
- `update_components/tile_component.cpp`
- `update_components/menu/*.cpp`

**4. Update All Graphics Component Implementations:**
- `graphics_components/camera_component.cpp`
- `graphics_components/world_component.cpp`
- `graphics_components/tile_component.cpp`
- `graphics_components/menu/*.cpp`

**5. Update GameObject Hierarchy:**
- `common/game_object.h/cpp` - base class signatures
- `game_interface.h/cpp` - propagate interface parameters
- `game_world.h/cpp` - propagate interface parameters
- `game_camera.h/cpp` - propagate interface parameters
- `menus/*.h/cpp` - propagate interface parameters

**Estimated Time:** 6-8 hours
**Risk:** Medium-High (touches many files, but mechanical changes)

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

1. **TODO:** Complete interface definitions in `src/graphics/`:
   - `ServiceLocator::LoadResources(ResourcesSystem& resources)`
   - `GameObject::HandleInput(InputSystem& input, CollisionSystem& collision)`
   - `GameObject::Update(CollisionSystem& collision)`
   - `GameObject::Render(RenderSystem& renderer)`

6. **TODO:** Update component base classes:
   - `InputComponent::HandleInput(GameObject&, InputSystem&, CollisionSystem&)`
   - `UpdateComponent::Update(GameObject&, CollisionSystem&)`
   - `GraphicsComponent::Render(GameObject&, RenderSystem&)`

7. **TODO:** Update main game loop in `game.cpp`:
   - Pass `graphics` as interface reference to each method
   - `interface.HandleInput(graphics, graphics)`
   - `interface.Update(graphics)`
   - `interface.Render(graphics)`

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
- [x] All interfaces defined with abstract methods (InputSystem, CollisionSystem, RenderSystem, ResourcesSystem)
- [x] Interfaces use only custom types (Position2D, Color2D, Rectangle2D, etc.) - NO raylib types
- [x] Graphics class implements all four interfaces
- [x] InputSystem methods: GetMousePosition, IsKeyPressed, IsMouseButtonPressed, IsKeyDown, GetMouseWheelMove
- [x] CollisionSystem methods: CheckCollisionPointRec, CheckCollisionRecs
- [x] RenderSystem methods: BeginDrawing, EndDrawing, BeginMode2D, EndMode2D, DrawRectangle, DrawDiamondFrame
- [x] ResourcesSystem methods: LoadTexture/Image, Unload, ImageCrop/Draw, GenImageColor, GetImageWidth/Height
- [x] Build succeeds with all interface implementations

**Phase 3 (Update Call Sites):**
- [ ] ServiceLocator::LoadResources takes ResourcesSystem& parameter
- [ ] GameObject::HandleInput takes InputSystem& and CollisionSystem& parameters
- [ ] GameObject::Update takes CollisionSystem& parameter
- [ ] GameObject::Render takes RenderSystem& parameter
- [ ] Component base classes updated with interface parameters
- [ ] Main game loop passes graphics as interface references
- [ ] All InputComponent implementations updated
- [ ] All UpdateComponent implementations updated
- [ ] All GraphicsComponent implementations updated
- [ ] All GameObject descendants updated (GameInterface, GameWorld, GameCamera, menus)
- [ ] No direct raylib calls in game logic files
- [ ] Build succeeds with new interface system

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
