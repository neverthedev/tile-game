# TASK-01: Replace .cpp includes with proper headers

**Priority:** 🔴 Critical
**Status:** ✅ DONE
**Estimated Effort:** High (affects entire codebase)
**Completed:** January 16, 2026

---

## Problem Description

The project currently includes `.cpp` files directly instead of using proper header files:

```cpp
#include "graphics.cpp"
#include "common/game_object.cpp"
#include "menus/factory.cpp"
```

### Why This Is Wrong

1. **Multiple Definition Errors**: When multiple source files include the same `.cpp`, the linker sees duplicate symbol definitions
2. **Compilation Performance**: Each `.cpp` inclusion recompiles all code, significantly slowing build times
3. **Violates C++ Best Practices**: Headers should contain declarations, implementations should be in `.cpp` files
4. **Header Guard Issues**: Using `#ifndef` guards in `.cpp` files masks the real problem
5. **Circular Dependencies**: Makes it harder to manage dependencies and can lead to inclusion cycles
6. **IDE Confusion**: Most IDEs expect `.h`/`.hpp` for declarations and `.cpp` for implementations

### Current Impact

- Fragile build system
- Slower compilation
- Potential for hard-to-debug linker errors
- Poor code organization
- Difficult for new developers to understand

---

## Desired Outcome

### Proper File Structure

Each class should have:
- **Header file** (`.h` or `.hpp`): Class declarations, inline functions
- **Implementation file** (`.cpp`): Class implementations

Example transformation:

**Before:**
```cpp
// graphics.cpp
#ifndef GRAPHICS_H
#define GRAPHICS_H

class Graphics {
  public:
    Graphics(int w, int h);
    void Draw();
  private:
    int width;
};

Graphics::Graphics(int w, int h): width{w} {}
void Graphics::Draw() { /* ... */ }

#endif
```

**After:**
```cpp
// graphics.h
#pragma once

class Graphics {
  public:
    Graphics(int w, int h);
    void Draw();
  private:
    int width;
};

// graphics.cpp
#include "graphics.h"

Graphics::Graphics(int w, int h): width{w} {}
void Graphics::Draw() { /* ... */ }
```

---

## Design Decisions

### Header Guard Style
✅ **Use `#pragma once`**
- Simpler, less code clutter
- Supported by all modern compilers
- No need for unique guard names

### File Extension
✅ **Use `.h` extension**
- Standard for C++ projects
- Simpler than `.hpp`
- We're not mixing C/C++ code

### Implementation Location
✅ **Keep ALL implementations in .cpp files**
- Headers contain only declarations (pure interface)
- All function implementations go in `.cpp` files, even simple getters/setters
- Cleaner headers, easier to read interface
- Faster recompilation (implementation changes don't trigger header recompilation)

Example:
```cpp
// game_object.h
#pragma once

class GameObject {
public:
    // Only declarations in header
    int GetX() const;
    void SetX(int newX);
    void Update(float deltaTime);

private:
    int x;
};

// game_object.cpp
#include "game_object.h"

// All implementations in .cpp
int GameObject::GetX() const { return x; }
void GameObject::SetX(int newX) { x = newX; }
void GameObject::Update(float deltaTime) {
    // Complex implementation here
}
```

### Include Order Convention
✅ **Standard include order** (to be followed in all files):
```cpp
// 1. Related header first (for .cpp files)
#include "graphics.h"

// 2. C++ standard library
#include <vector>
#include <string>

// 3. External libraries (raylib, etc.)
#include "raylib.h"

// 4. Project headers
#include "game_object.h"
#include "common/position_2d.h"
```

### Forward Declarations
✅ **Keep forward declarations local**
- Each header forward-declares what it needs
- No central forward declaration file
- More maintainable, clearer dependencies

### Migration Strategy
✅ **Incremental approach**
- Convert one file at a time
- Keep build working after each conversion
- Test after each step
- Easier to debug issues

### C Code Encapsulation
✅ **Minimize pure C usage**
- Native C code should be encapsulated in dedicated files
- Provide C++ interface wrappers where needed
- Keep C dependencies isolated

---

## Implementation Steps

### 1. Create Header Files
For each `.cpp` file that's being included:
- Create corresponding `.h` file (co-located with .cpp)
- Add `#pragma once` at the top
- Move class declarations to header (interface only)
- Keep ALL implementations in `.cpp` file (including simple getters/setters)

### 2. Update Includes
Replace:
```cpp
#include "graphics.cpp"
```

With:
```cpp
#include "graphics.h"
```

Follow the standard include order convention in all files.

### 3. Update CMakeLists.txt
Ensure all `.cpp` files are added to compilation:
```cmake
add_executable(game
  src/game.cpp
  src/graphics.cpp
  src/game_interface.cpp
  # ... all .cpp files
)
```

### 4. Fix Forward Declarations
Some files may need forward declarations to break circular dependencies:
```cpp
// In header
class Graphics;  // Forward declaration

// In .cpp
#include "graphics.h"
```

### 5. Create Documentation
After refactoring is complete:
- Document the new folder structure
- Document the include order convention
- Document the header/implementation separation (all implementations in .cpp)
- Create a "Code Organization Guide" for developers
- Include examples of proper header/implementation separation

---

## Files Affected

Based on grep results, files that need conversion:
- `src/graphics.cpp` → `src/graphics.h` + `src/graphics.cpp`
- `src/common/game_object.cpp` → `src/common/game_object.h` + `src/common/game_object.cpp`
- `src/common/position_2d.cpp`
- `src/common/rectangle_2d.cpp`
- `src/common/game_area.cpp`
- `src/common/game_error.cpp`
- `src/menus/menu.cpp`
- `src/menus/factory.cpp`
- `src/menus/decoration_menu.cpp`
- `src/managers/tiles_manager.cpp`
- `src/game_world.cpp`
- `src/game_interface_.cpp`
- `src/game_camera.cpp`
- All component files

---

## Testing Approach

1. Convert one file at a time (start with leaf dependencies)
2. Build after each conversion
3. Fix compilation errors
4. Verify program still runs correctly
5. Continue with next file

### Suggested Order

1. Start with common utilities (no dependencies):
   - Position2D
   - Rectangle2D
   - GameError

2. Move to base classes:
   - GameObject
   - Component base classes

3. Then concrete implementations:
   - Graphics
   - TilesManager
   - Menus

4. Finally, high-level classes:
   - GameWorld
   - GameInterface
   - main

---

## Benefits After Completion

✅ Proper C++ file structure
✅ Faster compilation times
✅ No multiple definition risks
✅ Better IDE support
✅ Easier to understand dependencies
✅ Better code organization
✅ Industry-standard practices

---

## Notes

- This is the **highest priority** task as it affects the entire project structure
- All other refactoring should wait until this is complete
- May uncover hidden dependency issues that need resolution
- Follow the design decisions documented above consistently
- Documentation of the new structure is part of this task completion

---

## References

- [C++ Core Guidelines - SF.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#sf1-use-a-cpp-suffix-for-code-files-and-h-for-interface-files-if-your-project-doesnt-already-follow-another-convention)
- [Google C++ Style Guide - Header Files](https://google.github.io/styleguide/cppguide.html#Header_Files)
