# TASK-09: Extract Hard-Coded Dependencies

**Priority:** 🟢 Medium
**Status:** ✅ DONE
**Estimated Effort:** Low-Medium

---

## Problem Description

Configuration values and dependencies are hard-coded:

```cpp
const float TILE_W = 64.0f;
const float TILE_H = 32.0f;
const int SCREEN_W = 1024;
const int SCREEN_H = 768;
const int FRAME_RATE = 60;

GameWorld gameWorld { GameWorld::NewWorld(60, 80, tilesManager) };  // Magic numbers
```

### Why This Is Wrong

1. **Hard to Change**: Requires recompilation
2. **No Configuration Files**: Can't adjust without code changes
3. **Magic Numbers**: Unclear what 60, 80 represent
4. **Not Testable**: Can't test with different configurations
5. **No User Control**: Players can't adjust settings

---

## Desired Outcome

Centralized, configurable settings system.

### Configuration Class

```cpp
// src/config/game_config.h
class GameConfig {
  public:
    // Display settings
    int ScreenWidth = 1024;
    int ScreenHeight = 768;
    int FrameRate = 60;
    bool Fullscreen = false;

    // Tile settings
    float TileWidth = 64.0f;
    float TileHeight = 32.0f;

    // World settings
    int WorldWidth = 60;
    int WorldHeight = 80;

    // Load from file
    static GameConfig LoadFromFile(const std::string& path);
    void SaveToFile(const std::string& path) const;

    // Load from JSON
    static GameConfig LoadFromJson(const std::string& json);
    std::string ToJson() const;
};
```

### Configuration File (JSON)

```json
{
  "display": {
    "screenWidth": 1024,
    "screenHeight": 768,
    "frameRate": 60,
    "fullscreen": false
  },
  "world": {
    "tileWidth": 64.0,
    "tileHeight": 32.0,
    "worldWidth": 60,
    "worldHeight": 80
  },
  "game": {
    "difficulty": "normal",
    "soundEnabled": true
  }
}
```

### Usage

```cpp
int main() {
  // Load configuration
  GameConfig config = GameConfig::LoadFromFile("config.json");

  // Use configuration
  Graphics graphics(config.ScreenWidth, config.ScreenHeight,
                   config.TileWidth, config.TileHeight,
                   "Game", config.FrameRate);

  auto world = GameWorld::NewWorld(config.WorldWidth,
                                   config.WorldHeight,
                                   tilesManager);
}
```

---

## Work Completed

1. Added `GameConfig` class with strict validation and JSON load/save.
2. Added nlohmann/json via CMake FetchContent.
3. Replaced hard-coded values in `game.cpp` and `game_interface.cpp`.
4. Added default `config/config.json` with window title.
5. Added config copy step in CMake.

## Decisions Made

- JSON library: nlohmann/json (FetchContent).
- Config location: `raylib-my/config/config.json` copied to build output.
- Global access: config stored in `ServiceLocator`.
- Validation: strict (missing/invalid fields are errors).

---

## Files Affected

- `src/game.cpp` - Use config instead of constants
- `src/config/game_config.h` (new)
- `src/config/game_config.cpp` (new)
- `config.json` (new)
- CMakeLists.txt - Copy config to build dir

---

## Benefits

✅ Runtime configuration
✅ No recompilation needed
✅ User-adjustable settings
✅ Easier testing
✅ Settings menu possible
✅ Multiple configurations
