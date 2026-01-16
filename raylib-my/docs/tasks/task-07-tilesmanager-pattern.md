# TASK-07: Improve TilesManager Pattern

**Priority:** 🟢 Medium
**Status:** 📋 TODO
**Estimated Effort:** Low-Medium

---

## Problem Description

`TilesManager` is passed around everywhere but lacks clear initialization guarantees and access patterns:

```cpp
TilesManager tilesManager;
GameWorld gameWorld { GameWorld::NewWorld(60, 80, tilesManager) };
```

Acting as a pseudo-singleton but without singleton guarantees.

### Why This Is Wrong

1. **Unclear Initialization Order**: No guarantee manager is initialized before use
2. **Passed by Reference Everywhere**: Suggests shared resource but not explicit
3. **Not True Singleton**: Multiple instances possible
4. **No Thread Safety**: If needed later
5. **Lifecycle Unclear**: When is it created/destroyed?

---

## Desired Outcome

Choose and implement a clear pattern for TilesManager.

### Option 1: Proper Singleton (If globally needed)

```cpp
class TilesManager {
  public:
    static TilesManager& Instance() {
      static TilesManager instance;
      return instance;
    }

    // Delete copy/move
    TilesManager(const TilesManager&) = delete;
    TilesManager& operator=(const TilesManager&) = delete;

  private:
    TilesManager() = default;
};

// Usage
TilesManager::Instance().LoadTextures(graphics);
```

### Option 2: Dependency Injection (Recommended)

```cpp
class GameWorld {
  public:
    GameWorld(std::shared_ptr<TilesManager> tilesManager)
      : tilesManager_(tilesManager) {}

  private:
    std::shared_ptr<TilesManager> tilesManager_;
};

// In main
auto tilesManager = std::make_shared<TilesManager>();
tilesManager->LoadTextures(graphics);

GameWorld world(tilesManager);
DecorationMenu menu(tilesManager);
```

### Option 3: Service Locator Pattern

```cpp
class ServiceLocator {
  public:
    static void ProvideTilesManager(std::shared_ptr<TilesManager> manager) {
      tilesManager_ = manager;
    }

    static TilesManager& GetTilesManager() {
      if (!tilesManager_) throw std::runtime_error("TilesManager not provided");
      return *tilesManager_;
    }

  private:
    static std::shared_ptr<TilesManager> tilesManager_;
};
```

---

## Implementation Steps

1. Decide on pattern based on needs
2. Update TilesManager class
3. Update all usage sites
4. Add initialization checks
5. Document lifecycle

---

## Files Affected

- `src/managers/tiles_manager.cpp`
- `src/game_interface_.cpp`
- `src/game_world.cpp`
- `src/menus/decoration_menu.cpp`

---

## Benefits

✅ Clear lifecycle management
✅ Guaranteed initialization
✅ Explicit dependencies
✅ Easier testing
✅ Thread-safe (if needed)
