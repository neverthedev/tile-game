# TASK-08: Separate Game State from View State

**Priority:** 🟢 Medium
**Status:** 📋 TODO
**Estimated Effort:** High

---

## Problem Description

Game state (world data, tile positions) is mixed with view state (camera position, rendering info):

```cpp
class GameWorld : public GameObject {
  GameCamera* camera;  // View state in game state!
  std::vector<WorldTile> grid;  // Game state
  // ...
};
```

### Why This Is Wrong

1. **Violates MVC/MVP**: Model contains view concerns
2. **Hard to Test**: Can't test game logic without rendering
3. **No Headless Mode**: Simulation requires graphics
4. **No Replay System**: Can't separate input from state
5. **Difficult Serialization**: Can't save just game state

---

## Desired Outcome

Separate pure game state from presentation concerns.

### Architecture

```cpp
// Pure game state (no rendering dependencies)
class WorldState {
  public:
    int MapWidth;
    int MapHeight;

    WorldTile& GetTile(Position2D pos);
    void UpdateTiles(float deltaTime);
    void PlaceDecoration(Position2D pos, DecorationType type);

    // Serializable
    void SaveTo(std::ostream& out);
    void LoadFrom(std::istream& in);

  private:
    std::vector<WorldTile> grid_;
};

// View state
class WorldView {
  public:
    void SetCamera(const Camera2D& camera);
    void SetTilesManager(const TilesManager& manager);
    void Render(const WorldState& state, Graphics& graphics);

  private:
    Camera2D camera_;
    const TilesManager* tilesManager_;
};

// Combined for convenience
class GameWorld {
  public:
    void HandleInput();
    void Update();
    void Render(Graphics& graphics);

    WorldState& GetState() { return state_; }
    WorldView& GetView() { return view_; }

  private:
    WorldState state_;
    WorldView view_;
};
```

---

## Benefits

✅ Testable game logic
✅ Headless simulation
✅ Replay systems possible
✅ Easy serialization
✅ Network multiplayer ready
✅ Better architecture

---

## Implementation Steps

1. Extract WorldState class
2. Extract WorldView class
3. Update GameWorld to use both
4. Test state changes independently
5. Enable headless mode

---

## Files Affected

- `src/game_world.cpp` - Major refactoring
- `src/game_interface_.cpp`
- Test files (enable state testing)
