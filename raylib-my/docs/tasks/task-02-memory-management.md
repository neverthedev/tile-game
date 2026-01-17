# TASK-02: Fix Manual Memory Management

**Priority:** 🔴 Critical
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

The code uses manual memory management with raw pointers and explicit `new`/`delete` **throughout the codebase**:

### Example 1: GameInterface - GameArea management
```cpp
void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  // Need to find out how works move - copy constructors in relation to vectors
  // And use it here to avoid calling new explicitly
  // Also need to find out how it would work in case of deletion from vektor.
  // How will it clean memory in this case.
  GameArea* area = new GameArea(obj, pos, priority);
  gameAreas.push_back(area);
  // ...
}

GameInterface::~GameInterface() {
  for(GameArea* area : gameAreas) {
    delete area;
  }
}
```

### Example 2: MenuFactory - Component creation
```cpp
DecorationMenu& MenuFactory::CreateDecorationMenu(Rectangle2D pos, TilesManager& mngr) {
  DecorationMenuInputComponent* inp_cmp = new DecorationMenuInputComponent();
  DecorationMenuUpdateComponent* upd_cmp = new DecorationMenuUpdateComponent();
  DecorationMenuGraphicsComponent* grph_cmp = new DecorationMenuGraphicsComponent();
  DecorationMenu* decorationMenu = new DecorationMenu(pos, mngr, *inp_cmp, *upd_cmp, *grph_cmp);
  return *decorationMenu;
}
```

### Example 3: GameWorld - Camera and components
```cpp
GameWorld::GameWorld(...) {
  CameraInputComponent * cameraInp = new CameraInputComponent;
  CameraGraphicsComponent * cameraGrph = new CameraGraphicsComponent;
  camera = new GameCamera(*cameraInp, *cameraGrph);
  // ...
}

GameWorld::~GameWorld() {
  delete &input;
  delete &render;
}
```

### Example 4: Factory methods returning raw pointers
```cpp
WorldTile* WorldTileTerrainType::NewTile(Position2D pos) const {
  return new WorldTile(*this, pos);
}
```

### Why This Is Wrong

1. **Memory Leak Risk**: If an exception occurs between `new` and `push_back`, memory leaks
2. **Exception Unsafety**: Destructor might not be called if exceptions occur
3. **Manual Lifetime Management**: Requires explicit `delete` in destructor
4. **Unclear Ownership**: Who owns the pointer? Can it be shared?
5. **Developer Uncertainty**: Comment shows confusion about proper C++ patterns
6. **No RAII**: Resource Acquisition Is Initialization principle not followed

### Current Impact

- Potential memory leaks
- Crash risks if exception thrown
- Difficult to maintain
- Error-prone pattern
- Your own comment acknowledges the problem

---

## Checklist - Fix RAII Violations

### GameInterface - GameArea Management
- [x] Update `gameAreas` vector to use smart pointers or value semantics
- [x] Modify `AddArea()` method to use `std::make_unique` or `emplace_back`
- [x] Remove manual `delete` loop from destructor
- [x] Update all usage sites that access `gameAreas`
- [x] Implement index-based sorting to avoid moving objects with reference members
- [x] Document solution in [game_interface_sorting.md](../game_interface_sorting.md)

### MenuFactory - Component Creation
- [ ] Convert component pointers to smart pointers or value semantics
- [ ] Update `CreateDecorationMenu()` to use RAII
- [ ] Ensure Menu class properly owns its components
- [ ] Update Menu destructor if needed

### GameWorld - Camera Components
- [ ] Convert camera component pointers to smart pointers or value semantics
- [ ] Update camera creation in constructor
- [ ] Remove manual `delete &input` and `delete &render` from destructor
- [ ] Update `NewWorld()` factory method

### GameCamera - Component References
- [ ] Store components as values or smart pointers (not references)
- [ ] Remove manual `delete &input` and `delete &graphic` from destructor
- [ ] Update constructor to accept proper ownership

### Menu - Component References
- [ ] Store components as values or smart pointers (not references)
- [ ] Remove manual `delete &input` and `delete &graphics` from destructor
- [ ] Update constructor to accept proper ownership

### WorldTile Factory
- [ ] Change `NewTile()` return type from `WorldTile*` to `std::unique_ptr<WorldTile>`
- [ ] Update all callers to handle smart pointer return
- [ ] Consider if value semantics would be better

---

## Desired Outcome

Use modern C++ smart pointers and RAII principles:

### Option 1: Unique Ownership (Recommended)

```cpp
class GameInterface: public GameObject {
  private:
    std::vector<std::unique_ptr<GameArea>> gameAreas;
};

void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  auto area = std::make_unique<GameArea>(obj, pos, priority);
  gameAreas.push_back(std::move(area));

  std::sort(gameAreas.begin(), gameAreas.end(),
    [](const std::unique_ptr<GameArea>& a1, const std::unique_ptr<GameArea>& a2) {
      return a1->priority < a2->priority;
    });
}

// Destructor automatically handles cleanup
GameInterface::~GameInterface() = default;
```

### Option 2: Value Semantics (If GameArea is copyable/movable)

```cpp
class GameInterface: public GameObject {
  private:
    std::vector<GameArea> gameAreas;
};

void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  gameAreas.emplace_back(obj, pos, priority);

  std::sort(gameAreas.begin(), gameAreas.end(),
    [](const GameArea& a1, const GameArea& a2) {
      return a1.priority < a2.priority;
    });
}

// Destructor automatically handles cleanup
GameInterface::~GameInterface() = default;
```

### Option 3: Shared Ownership (If areas can be shared)

```cpp
class GameInterface: public GameObject {
  private:
    std::vector<std::shared_ptr<GameArea>> gameAreas;
};

void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  auto area = std::make_shared<GameArea>(obj, pos, priority);
  gameAreas.push_back(area);
  // Can be shared with other systems if needed
}
```

---

## Implementation Steps

### 1. Analyze GameArea Requirements
Determine:
- Is GameArea copyable/movable?
- Should ownership be unique or shared?
- Are there performance concerns with copying?

### 2. Choose Appropriate Smart Pointer

**Use `unique_ptr` if:**
- Single owner
- No sharing needed
- Lightweight, zero overhead

**Use `shared_ptr` if:**
- Multiple owners possible
- Need reference counting
- Slight overhead acceptable

**Use value semantics if:**
- Small objects
- Copyable/movable
- Best performance

### 3. Update GameInterface

```cpp
// In game_interface.h
class GameInterface {
  private:
    std::vector<std::unique_ptr<GameArea>> gameAreas;
    // or std::vector<GameArea> gameAreas;
};
```

### 4. Update AddArea Method

Remove explicit `new` and `delete`:
```cpp
void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  gameAreas.push_back(std::make_unique<GameArea>(obj, pos, priority));
  // or gameAreas.emplace_back(obj, pos, priority);

  std::sort(gameAreas.begin(), gameAreas.end(),
    [](const auto& a1, const auto& a2) {
      return a1->priority < a2->priority;  // or a1.priority < a2.priority
    });
}
```

### 5. Remove Manual Destructor

```cpp
// This can be removed or defaulted
GameInterface::~GameInterface() = default;
```

### 6. Update Usage Sites

Dereference smart pointers where needed:
```cpp
// Before
GameArea* gameArea = gameAreas[i];

// After (with unique_ptr)
GameArea* gameArea = gameAreas[i].get();
// or
auto& gameArea = *gameAreas[i];
```

---

## Files Affected

### Primary Files with RAII Violations:

1. **`src/game_interface.cpp`** - GameArea vector management
   - Line 22: `new GameArea`
   - Line 65: `delete area` in destructor

2. **`src/menus/factory.cpp`** - Menu and component creation
   - Lines 8-12: Multiple `new` calls for components and menu

3. **`src/game_world.cpp`** - Camera and world components
   - Lines 15-17: Camera components creation
   - Lines 87-88: World components in factory method
   - Lines 79-80: Manual deletion of references

4. **`src/game_camera.cpp`** - Component cleanup
   - Lines 36-37: Manual deletion of component references

5. **`src/menus/menu.cpp`** - Component cleanup
   - Lines 25-26: Manual deletion of component references

6. **`src/world_tiles/tile.cpp`** - Factory method
   - Line 48: Returns raw pointer from `NewTile()`

### Related Headers (may need updates):
- `src/game_interface.h`
- `src/menus/factory.h`
- `src/game_world.h`
- `src/game_camera.h`
- `src/menus/menu.h`
- `src/world_tiles/tile.h`

---

## Testing Approach

1. Update to smart pointers
2. Compile and fix any errors
3. Run the application
4. Verify no memory leaks using:
   - Valgrind (Linux)
   - Instruments (macOS)
   - Visual Studio Memory Profiler (Windows)

```bash
# On macOS/Linux with valgrind
valgrind --leak-check=full ./build/game

# On macOS with leaks
leaks --atExit -- ./build/game
```

---

## Benefits After Completion

✅ No manual memory management
✅ Exception-safe code
✅ Automatic cleanup
✅ Clear ownership semantics
✅ RAII compliance
✅ Modern C++ practices
✅ Reduced bug potential
✅ Easier to maintain

---

## Common Pitfalls to Avoid

### ❌ Don't do this:
```cpp
unique_ptr<GameArea> area(new GameArea(...));  // Old style
```

### ✅ Do this instead:
```cpp
auto area = make_unique<GameArea>(...);  // Exception-safe
```

### ❌ Don't mix ownership:
```cpp
GameArea* raw = new GameArea(...);
unique_ptr<GameArea> smart(raw);  // Dangerous!
```

---

## Understanding Move Semantics

Since the comment mentions confusion about move/copy:

```cpp
// Vector with unique_ptr uses move semantics
vector<unique_ptr<GameArea>> areas;
auto area = make_unique<GameArea>(...);
areas.push_back(std::move(area));  // Ownership transferred
// area is now nullptr

// Vector with value types uses copy/move
vector<GameArea> areas;
GameArea area(...);
areas.push_back(area);        // Copy
areas.push_back(std::move(area));  // Move (if move constructor exists)
areas.emplace_back(...);      // Construct in-place (best performance)
```

---

## References

- [C++ Core Guidelines - R.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-raii)
- [C++ Core Guidelines - R.20](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-owner)
- [Smart Pointers (cppreference)](https://en.cppreference.com/w/cpp/memory)
