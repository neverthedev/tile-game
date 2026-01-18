# TASK-04: Clarify Component Ownership

**Priority:** 🟡 High
**Status:** ✅ COMPLETE
**Completed:** January 18, 2026

---

## Summary

All ownership concerns have been successfully addressed throughout the codebase. The project now uses modern C++ smart pointers consistently, with clear ownership semantics and automatic lifetime management.

**Major Achievement:** Resolved manager reference issues by implementing Service Locator pattern, eliminating all non-owning references and initialization order dependencies.

---

## ✅ Solutions Implemented

### 1. Component Ownership (Pre-existing)

Game components (`InputComponent`, `GraphicsComponent`, `UpdateComponent`) use `std::unique_ptr`:

```cpp
class GameObject {
protected:
  std::unique_ptr<InputComponent> inputComponent;
  std::unique_ptr<GraphicsComponent> graphicsComponent;
  std::unique_ptr<UpdateComponent> updateComponent;
};

// Usage
auto camera = std::make_unique<GameCamera>(
  std::make_unique<CameraInputComponent>(),
  std::make_unique<CameraGraphicsComponent>(),
  std::make_unique<CameraUpdateComponent>()
);
```

**Benefits:**
- GameObject owns its components
- Automatic cleanup when GameObject destroyed
- No dangling references
- Clear ownership transfer semantics

### 2. Menu Factory Returns unique_ptr (Fixed Jan 18, 2026)

**Files Modified:**
- [menus/factory.h](../../src/menus/factory.h)
- [menus/factory.cpp](../../src/menus/factory.cpp)

**Before:**
```cpp
// Memory leak - returns reference to heap-allocated object
DecorationMenu& MenuFactory::CreateDecorationMenu(Rectangle2D pos, TilesManager& mngr) {
  DecorationMenu* decorationMenu = new DecorationMenu(...);
  return *decorationMenu;  // Who owns this?
}
```

**After:**
```cpp
// Returns unique_ptr with clear ownership transfer
std::unique_ptr<DecorationMenu> MenuFactory::CreateDecorationMenu(
  Rectangle2D pos,
  TilesManager& mngr
) {
  auto inp_cmp = std::make_unique<DecorationMenuInputComponent>();
  auto upd_cmp = std::make_unique<DecorationMenuUpdateComponent>();
  auto grph_cmp = std::make_unique<DecorationMenuGraphicsComponent>();

  return std::make_unique<DecorationMenu>(
    pos,
    std::move(inp_cmp),
    std::move(upd_cmp),
    std::move(grph_cmp),
    mngr
  );
}
```

### 3. GameInterface Owns Menu (Fixed Jan 18, 2026)

**Files Modified:**
- [game_interface.h](../../src/game_interface.h)
- [game_interface.cpp](../../src/game_interface.cpp)

**Before:**
```cpp
// Unclear ownership
class GameInterface {
  Menu* currentMenu;  // Who owns this? When is it deleted?
};

currentMenu = &MenuFactory::CreateDecorationMenu(...);  // Memory leak!
```

**After:**
```cpp
// Clear ownership
class GameInterface {
  std::unique_ptr<Menu> currentMenu;  // GameInterface owns the menu
};

// Direct assignment from factory
currentMenu = MenuFactory::CreateDecorationMenu({
  float(screenWidth) - 160, 0, 160.0, float(screenHeight)
}, tilesManager);
```

### 4. DecorationMenu Constructor Made Public (Jan 18, 2026)

**File Modified:** [menus/decoration_menu.h](../../src/menus/decoration_menu.h)

Changed from `protected` to `public` to allow `std::make_unique` to work directly (cleaner than using `new` with friend access).

```cpp
class DecorationMenu: public Menu {
public:  // Changed from protected
  DecorationMenu(
    Rectangle2D,
    std::unique_ptr<InputComponent>,
    std::unique_ptr<UpdateComponent>,
    std::unique_ptr<GraphicsComponent>
  );
  // ...
};
```

### 5. Service Locator Pattern for Global Resources (Jan 18, 2026)

**Problem:** TilesManager was passed by non-owning reference through multiple layers, creating initialization order dependencies and lifetime coupling.

**Original Call Chain (Problematic):**
```
main.cpp (local TilesManager)
  └─ GameInterface(TilesManager&)
       └─ GameWorld(TilesManager&)
            └─ DecorationMenu(TilesManager&)
```

**Issues Identified:**
- Non-owning references obscured actual ownership
- Created initialization order dependencies
- Tightly coupled all components to TilesManager lifetime
- Made testing and mocking difficult
- Manual parameter passing through all layers

**Solution: Service Locator Pattern**

Implemented centralized resource management:

```cpp
// services/service_locator.h
class ServiceLocator {
public:
    static void Initialize();
    static void Shutdown();
    static void LoadResources(Graphics& graphics);
    static TilesManager& GetTilesManager();

private:
    static std::unique_ptr<TilesManager> tilesManager_;
    static bool initialized_;
};
```

**New Initialization Flow:**
```
main.cpp
  └─ ServiceLocator::Initialize()              // Create TilesManager
  └─ Graphics graphics
  └─ ServiceLocator::LoadResources(graphics)   // Load textures
  └─ GameInterface interface                   // No TilesManager parameter
       └─ GameWorld world                      // No TilesManager parameter
            └─ Uses ServiceLocator::GetTilesManager()
```

**Files Modified:**
- Created [services/service_locator.h](../../src/services/service_locator.h)
- Created [services/service_locator.cpp](../../src/services/service_locator.cpp)
- Moved `managers/tiles_manager.*` → [services/tiles_manager.*](../../src/services/tiles_manager.h)
- Updated [game_interface.h](../../src/game_interface.h) - removed TilesManager member
- Updated [game_world.h](../../src/game_world.h) - removed TilesManager parameter
- Updated [menus/decoration_menu.h](../../src/menus/decoration_menu.h) - removed TilesManager parameter
- Updated [menus/factory.h](../../src/menus/factory.h) - removed TilesManager parameter
- Updated [game.cpp](../../src/game.cpp) - added ServiceLocator initialization

**Benefits:**
- Clear ownership: ServiceLocator owns TilesManager via unique_ptr
- No initialization order issues: Explicit Initialize() → LoadResources() → Usage
- No lifetime coupling: Components don't hold references
- Testable: Can mock ServiceLocator in tests
- Clean signatures: No TilesManager parameters polluting interfaces

See [SERVICE_LOCATOR.md](../SERVICE_LOCATOR.md) for detailed architecture diagrams.

---

## Benefits Achieved

✅ **Clear ownership semantics** - All heap-allocated objects have explicit owners
✅ **Automatic cleanup** - Smart pointers handle deallocation via RAII
✅ **No dangling references** - Lifetime guarantees enforced by type system
✅ **No memory leaks** - All allocations automatically cleaned up
✅ **Self-documenting code** - Ownership visible in type signatures
✅ **Compiler-enforced safety** - Cannot accidentally copy unique_ptr
✅ **Modern C++ best practices** - Consistent use of smart pointers
✅ **No initialization order issues** - ServiceLocator provides explicit lifecycle
✅ **Decoupled architecture** - Components don't depend on manager references

---

## Files Changed

| File | Change | Date |
|------|--------|------|
| [menus/factory.h](../../src/menus/factory.h) | Return `std::unique_ptr`, remove TilesManager | Jan 18, 2026 |
| [menus/factory.cpp](../../src/menus/factory.cpp) | Use `std::make_unique` | Jan 18, 2026 |
| [menus/decoration_menu.h](../../src/menus/decoration_menu.h) | Make constructor public, remove TilesManager | Jan 18, 2026 |
| [game_interface.h](../../src/game_interface.h) | Store `std::unique_ptr<Menu>`, remove TilesManager | Jan 18, 2026 |
| [game_interface.cpp](../../src/game_interface.cpp) | Update menu initialization | Jan 18, 2026 |
| [services/service_locator.h](../../src/services/service_locator.h) | Created ServiceLocator | Jan 18, 2026 |
| [services/service_locator.cpp](../../src/services/service_locator.cpp) | Implemented ServiceLocator | Jan 18, 2026 |
| [services/tiles_manager.*](../../src/services/tiles_manager.h) | Moved from managers/ | Jan 18, 2026 |
| [game_world.h](../../src/game_world.h) | Remove TilesManager parameter | Jan 18, 2026 |
| [game_world.cpp](../../src/game_world.cpp) | Use ServiceLocator | Jan 18, 2026 |
| [game.cpp](../../src/game.cpp) | Add ServiceLocator initialization | Jan 18, 2026 |

---

## Testing Completed

✅ Code compiles without errors
✅ Application runs correctly
✅ No memory leaks (automatic cleanup verified)
✅ Menu lifecycle properly managed
✅ ServiceLocator initialization/shutdown tested
✅ TilesManager accessed correctly via ServiceLocator

---

## Ownership Patterns in Codebase

### Pattern 1: Unique Component Ownership
```cpp
class GameObject {
  std::unique_ptr<InputComponent> inputComponent;
  std::unique_ptr<GraphicsComponent> graphicsComponent;
  std::unique_ptr<UpdateComponent> updateComponent;
};
```

### Pattern 2: Factory with Ownership Transfer
```cpp
std::unique_ptr<DecorationMenu> MenuFactory::CreateDecorationMenu(...) {
  return std::make_unique<DecorationMenu>(...);
}
```

### Pattern 3: Service Locator for Global Resources
```cpp
// Application initialization
ServiceLocator::Initialize();  // Creates TilesManager
ServiceLocator::LoadResources(graphics);  // Loads textures

// Usage anywhere in codebase
TilesManager& tm = ServiceLocator::GetTilesManager();
auto tile = tm.NewTile("grass", position);
```

---

## References

- [C++ Core Guidelines - R.20: Use unique_ptr or shared_ptr to represent ownership](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rr-owner)
- [C++ Core Guidelines - F.7: For general use, take T* or T& arguments rather than smart pointers](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-smart)
- [Effective Modern C++ - Item 21: Prefer std::make_unique and std::make_shared](https://www.aristeia.com/books.html)
