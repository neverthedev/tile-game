# Code Organization Guide

This document describes the code organization standards for the raylib-my project.

## File Structure

### Header/Implementation Separation

All classes follow the standard C++ pattern:
- **Header files (`.h`)**: Contain class declarations (interface)
- **Implementation files (`.cpp`)**: Contain class implementations

```
src/
  common/
    position_2d.h          # Header with class declaration
    position_2d.cpp        # Implementation
    rectangle_2d.h
    rectangle_2d.cpp
    game_object.h
    game_object.cpp
    game_area.h
    game_area.cpp
    game_error.h
    game_error.cpp
  graphics.h
  graphics.cpp
  game_camera.h
  game_camera.cpp
  game_world.h
  game_world.cpp
  game_interface.h
  game_interface_.cpp        # Implementation for GameInterface
  managers/
    tiles_manager.h
    tiles_manager.cpp
  menus/
    menu.h
    menu.cpp
    decoration_menu.h
    decoration_menu.cpp
    factory.h
    factory.cpp
  input_components/
    component.h            # Base class header
    component.cpp
    camera_component.h     # Camera input handling
    camera_component.cpp
    world_component.h      # World input handling
    world_component.cpp
    menu/
      decoration_menu_component.h
      decoration_menu_component.cpp
  update_components/
    component.h
    component.cpp
    menu/
      decoration_menu_component.h
      decoration_menu_component.cpp
  graphics_components/
    component.h
    component.cpp
    camera_component.h     # Camera rendering
    camera_component.cpp
    world_component.h      # World rendering
    world_component.cpp
    empty_component.h      # Empty component (no-op)
    empty_component.cpp
    menu/
      decoration_menu_component.h
      decoration_menu_component.cpp
  world_tiles/
    tile.h
    tile.cpp
    tile_terrain_type.h
    tile_terrain_type.cpp
    decorations/
      decoration.h
      decoration.cpp
    resources/
      resource.h
      resource.cpp
```

## Code Style Standards

### Header Guards
Use `#pragma once` for all header files:

```cpp
#pragma once

// Header content here
```

**Why:** Simpler than traditional include guards, supported by all modern compilers.

### Indentation
**CRITICAL: Use 2-space indentation for all code.**

```cpp
class Example {
public:
  void Method() {
    if (condition) {
      DoSomething();
    }
  }
};
```

### Include Order

Follow this order in all files:

```cpp
// 1. Related header first (for .cpp files)
#include "my_class.h"

// 2. C++ standard library
#include <vector>
#include <string>

// 3. External libraries (raylib, etc.)
#include "raylib.h"

// 4. Project headers
#include "common/game_object.h"
#include "graphics.h"
```

**Why:** This order helps detect missing includes and dependencies early.

### Implementation Location

**ALL function implementations go in `.cpp` files**, even simple getters/setters.

```cpp
// example.h
#pragma once

class Example {
public:
  int GetValue() const;      // Declaration only
  void SetValue(int newVal);

private:
  int value;
};

// example.cpp
#include "example.h"

int Example::GetValue() const {
  return value;
}

void Example::SetValue(int newVal) {
  value = newVal;
}
```

**Benefits:**
- Cleaner headers (pure interface)
- Faster recompilation (changing implementation doesn't trigger header recompilation)
- Clear separation of concerns

### Forward Declarations

Use forward declarations in headers to avoid circular dependencies:

```cpp
// game_object.h
#pragma once

// Forward declaration
class Graphics;

class GameObject {
public:
  virtual void Render(Graphics&) = 0;  // Can use pointer/reference
};
```

Then include the full header in the `.cpp` file:

```cpp
// game_object.cpp
#include "game_object.h"
:

```cmake
file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE C_FILES CONFIGURE_DEPENDS *.c)
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.h *.hpp)

# All .cpp files are now proper compilation units
set(SOURCE_FILES ${CPP_FILES} ${C_FILES})

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})
```cpp

Every `.cpp` file is a standalone compilation unit - no files are excluded or `#include`d by
list(FILTER CPP_FILES EXCLUDE REGEX "game_interface_\\.cpp$")
list(FILTER CPP_FILES EXCLUDE REGEX "camera_component\\.cpp$")
list(FILTER CPP_FILES EXCLUDE REGEX "world_component\\.cpp$")
list(FILTER CPP_FILES EXCLUDE REGEX "menu/decoration_menu_component\\.cpp$")
list(FILTER CPP_FILES EXCLUDE REGEX "empty_component\\.cpp$")
```

**Why some files are excluded:** These files contain component implementations that need access to types defined later in the translation unit, so they are `#include`d at strategic points in other `.cpp` files.

## Best Practices

### 1. One Class Per File
Each class should have its own `.h` and `.cpp` file pair (with few exceptions for tightly coupled classes).

### 2. Minimize Header Dependencies
- Use forward declarations where possible
- Only include what you need
- Prefer including in `.cpp` over `.h`

### 3. Const Correctness
- Mark member functions `const` if they don't modify the object
- Use `const` references for parameters that aren't modified

### 4. RAII and Resource Management
- Constructors acquire resources
- Destructors release resources
- Avoid manual `new`/`delete` where possible (prefer smart pointers)

### 5. Naming Conventions
- **Classes**: PascalCase (e.g., `GameObject`, `TilesManager`)
- **Functions/Methods**: PascalCase (e.g., `HandleInput`, `GetValue`)
- **Variables**: camelCase (e.g., `tileWidth`, `mapHeight`)
- **Private members**: camelCase (e.g., `tilesManager`, `initialized`)

## Common Patterns

### Component Pattern
Components are used extensively for separating concerns:

```cpp
// Base component class
class InputComponent {
public:
  virtual void HandleInput(GameObject&) = 0;
};

// Concrete implementation
class CameraInputComponent: public InputComponent {
public:
  void HandleInput(GameObject&) override;
};
```

### Factory Pattern
Factories create complex objects with proper dependency injection:

```cpp
class MenuFactory {
public:
  static DecorationMenu& CreateDecorationMenu(Rectangle2D, TilesManager&);
};
```

## Migration Notes

This project was refactored from a pattern where `.cpp` files were included directly (which caused multiple definition errors and build issues) to proper header/implementation separation.

**Key changes made:**
1. Created `.h` files for all classes
2. Moved declarations to headers, implementations to `.cpp`
3. Updated all `#include "file.cpp"` to `#include "file.h"`
4. Updated CMakeLists.txt to compile all `.cpp` files
5. Added `#pragma once` to all headers
6. Established consistent include order

---

**Last Updated:** January 16, 2026
