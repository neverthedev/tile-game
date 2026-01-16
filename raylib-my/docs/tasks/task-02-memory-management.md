# TASK-02: Fix Manual Memory Management

**Priority:** 🔴 Critical
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

The code uses manual memory management with raw pointers and explicit `new`/`delete`:

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

- `src/game_interface_.cpp` (primary change)
- Any other files using raw `new`/`delete` for dynamic objects
- Check for similar patterns in:
  - Menu system
  - Component creation
  - Any factory methods

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
