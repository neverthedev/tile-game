# TASK-15: Save/Restore World State to/from File

**Priority:** 🟡 High
**Status:** 🚧 IN PROGRESS
**Estimated Effort:** Medium-High

---

## Problem Description

`GameWorld` is created procedurally at startup and cannot be persisted:

- The world grid is generated via a procedural `SimpleWorldGenerator`.
- There is no way to save the current world (tiles, decorations, resources, etc.) to disk.
- There is no way to restore a saved world from disk.

This blocks:
- Continuing a session later
- Sharing a world with others
- Reproducible debugging (load a known world state)

---

## Desired Outcome

Provide a stable, versioned save/load format that can round-trip:
- World size (width/height)
- Tile terrain type per cell
- Per-tile optional decoration + resource (and their state, if any)
- Optional: camera view state (offset/target/rotation/zoom)
- Save/load should be reachable via config + hotkeys (see Runtime Integration).

## Current Progress (as of January 31, 2026)

✅ Implemented (loading pipeline)
- `WorldDataReader` contract + DTOs (`WorldMeta`, `WorldTileData`, `CameraState`).
- `JsonFileStorage` (reader) that:
  - loads full JSON first, then validates (same pattern as `GameConfig`),
  - validates `saveVersion == 1` and `encoding` fields,
  - decodes Base64 blobs into typed arrays,
  - validates packed `resourceVolumes` / `decorationStates` sizes vs non-zero id counts,
  - streams tiles via `BeginTileScan()` + `NextTile()` (row-major).
- `WorldLoadService` that builds a new `GameWorld` from `WorldDataReader`, using:
  - a world builder in `WorldPersistenceService` to populate the grid,
  - `TilesManager::NewTile(tileTypeName, pos)` for terrain tiles,
  - decoration/resource reconstruction from ids + packed state.
- `GameWorld` now supports tile construction via `TileProvider` and stores tiles as `std::unique_ptr`.
- `WorldTile` now owns `Decoration` / `Resource` as `std::unique_ptr`.
- Shared utilities:
  - Base64 decoding extracted to `src/common/base64.*`
  - JSON “require” helpers extracted to `src/common/json_require.h`
- `SimpleWorldGenerator` implementing `WorldDataReader` for procedural worlds.
- `WorldPersistenceService` with load-or-generate policy (generate-only for now) and a `CreateFromServices()` factory.

🚧 Not implemented yet (still required for this task)
- Saving pipeline:
  - `WorldDataWriter` contract + `WorldSaver` to extract `GameWorld` into the format-neutral snapshot
  - `JsonFileStorage` (writer) to encode blobs + write JSON
- Runtime integration:
  - add config field `game.saveFile` (default `saves/world.json`)
  - `F5` save, `F6` load (and world swap in `GameInterface` without dangling `GameArea` references)
- Saving-side identifiers:
  - a stable way to read tile type name from a `WorldTile` (to build the type maps)

### Proposed File Format (JSON, versioned)

Reuse the existing `nlohmann/json` dependency (already used by `GameConfig`) for:
- Human-readable saves during development
- Strict validation + actionable errors

#### Schema v1 (tileTypes + binary tile map)

Do not store tiles as `{x,y}` objects. For large worlds this is too big and too slow to parse.

Instead:
- `tileTypes` maps tile type name → integer id.
- `tiles` stores the per-cell tile type id as a binary blob serialized in row-major order.
- Do the same for per-tile decoration/resource: define `decorationTypes` / `resourceTypes`, then store per-tile ids as binary.
- Use the `decorations` / `resources` arrays themselves as the mask: store extra state arrays only for tiles where the id is non-zero (packed, row-major scan order).
- Because the container is JSON, binary is stored as Base64 text.

Example schema (v1):

```json
{
  "saveVersion": 1,
  "world": {
    "width": 60,
    "height": 80,
    "tileTypes": {
      "Deep Water": 0,
      "Plains": 1,
      "Grassland": 2
    },
    "decorationTypes": {
      "Grass": 1,
      "Rock": 2,
      "Wall": 3,
      "Tree": 4,
      "Road": 5
    },
    "resourceTypes": {
      "Coil": 1,
      "Clay": 2,
      "Iron": 3,
      "Copper": 4
    },
    "encoding": {
      "order": "row-major",
      "valueType": "u16",
      "endianness": "little",
      "codec": "base64"
    },
    "tiles": "AAABAAIAAAEAAQACAAIA..."
    ,
    "decorations": "AAAAAAQAAAABAAIAAAAD..."
    ,
    "resources": "AAAAAAEAAAADAAAAAAIA..."
    ,
    "resourceVolumes": "AAAAAAoAAAAAFAAAAA=="
    ,
    "decorationStates": "AAAAAAEAAAACAAAAAA=="
  },
  "camera": {
    "offset": { "x": 0.0, "y": 0.0 },
    "target": { "x": 0.0, "y": 0.0 },
    "rotation": 0.0,
    "zoom": 1.0
  }
}
```

Notes:
- Store `x/y` as integer grid coordinates; reconstruct `WorldTile::Pos` using `x + 0.5f`, `y + 0.5f` to match current initialization.
- Tile position is implicit by index: `index = y * width + x`.
- `encoding` applies to `tiles`, `decorations`, `resources`, and `resourceVolumes` (do not repeat per-field).
- `resourceVolumes` uses `u32` even when `encoding.valueType` is `u16` (it’s always `u32 little-endian` in v1).
- `decorationStates` uses `u32 little-endian` in v1.
- `valueType` is `u16` to keep v1 simple and robust.
- `decorationTypeId == 0` and `resourceTypeId == 0` are reserved for “no decoration/resource”.
  - Type maps may omit id 0; the reader treats it as empty.
- `resourceVolumes` is packed: it contains values only for tiles where `resources[i] != 0`, in row-major scan order.
- `decorationStates` is packed: it contains values only for tiles where `decorations[i] != 0`, in row-major scan order.
- We can add `compression` later (e.g. `zstd`) as an optional, layered step: `raw bytes -> compress -> base64`.

---

## Runtime Integration (Config + Hotkeys)

### Config: Save File Path

Add a config field for the save file name/path. For v1:
- Default directory: `saves/`
- Default file: `saves/world.json`
- Config should allow changing the file path without recompiling.

Proposed config JSON addition:

```json
{
  "game": {
    "saveFile": "saves/world.json"
  }
}
```

Expected behavior:
- On startup, attempt to load `saveFile` (before creating a new world).
- If the file does not exist or fails validation, start a new world and keep the error actionable (log/console for now).
- When saving, ensure the directory exists (create `saves/` if needed).

### Hotkeys

- `F5`: save world to `saveFile`
- `F6`: load world from `saveFile` (replaces current world instance)

Notes:
- `InputSystem` already supports `IsKeyPressed(Keyboard2D::Key)`, but `Keyboard2D` currently defines only arrows + WASD.
- Add `Keyboard2D::KEY_F5` and `Keyboard2D::KEY_F6` constants (matching raylib key codes) so code can use `input.IsKeyPressed(Keyboard2D::KEY_F5)` and `input.IsKeyPressed(Keyboard2D::KEY_F6)`.

---

## Design Constraints / Required API Changes

To serialize and restore correctly, the save layer needs stable identifiers:

1. **Terrain type identifier**
   - Save by terrain name (e.g. `"Grassland"`) because `TilesManager` already uses `std::string` keys.
   - Add a way to read a tile’s terrain name at runtime.
     - Option A: expose a `Name()` getter on `WorldTileTerrainType`.
     - Option B: store `terrainTypeName` on `WorldTile` when created.

2. **Decoration/resource representation**
   - Decide how these are created/owned (now `std::unique_ptr` on `WorldTile`).
   - Save as either:
     - Enum values as strings (`"Tree"`, `"Iron"`) + optional fields (e.g. `volume`)
     - Or as fully qualified names if those are the true identifiers.

3. **World swapping**
   - `GameInterface` stores `gameWorld` as `std::unique_ptr<GameWorld>`, but `GameArea` stores references to `GameObject`.
   - Loading a new world requires rebuilding or updating the `GameInterface` areas list to avoid dangling references.
   - `F6` load should replace the world at runtime (not only at startup), so this refactor is required for v1.

---

## Implementation Steps

1. **Define format-neutral contract + JSON IO**
  - ✅ `WorldDataReader` + DTOs (`WorldMeta`, `WorldTileData`, `CameraState`).
  - ✅ `JsonFileStorage` reader (LoadFromJson + ValidateLoadedData, Base64 decode).
  - ⬜ Add `WorldDataWriter` interface and implement writer side in `JsonFileStorage`.
  - ⬜ Implement Base64 encode (decode exists).

2. **Implement generation + startup policy (load-or-generate)**
   - ✅ Create `SimpleWorldGenerator` that implements `WorldDataReader` for procedural worlds.
   - ✅ Move current procedural logic from `GameWorld` factory into `SimpleWorldGenerator`.
   - ✅ Create `WorldPersistenceService` and make startup use it:
     - `LoadWorld()` uses `JsonFileStorage` + `WorldLoadService`.
     - `GenerateWorld()` uses `SimpleWorldGenerator` + `WorldLoadService`.
     - `LoadOrGenerate()` selects the strategy (try load; on failure generate).
     - `SaveWorld()` exists but is a stub/disabled until saving pipeline is completed.

3. **Serialize GameWorld → WorldSave**
   - Iterate tiles in a deterministic order (row-major).
   - Capture:
     - `width`, `height`
     - `tileTypes` mapping (name → id)
     - per-tile type id encoded as binary (`u16 little-endian`, row-major), Base64 in JSON
     - `decorationTypes` mapping (name → id), plus per-tile decoration id array (`u16`, Base64)
     - `resourceTypes` mapping (name → id), plus per-tile resource id array (`u16`, Base64)
     - resource state fields:
       - packed volume array (`u32`, Base64) for `WorldTileResource::Volume()` only where `resourceId != 0`
     - decoration state fields:
       - packed decoration state array (`u32`, Base64) only where `decorationId != 0`
     - optional camera state
   - ⬜ Not implemented (requires stable “tile type name” getter on `WorldTile` / terrain type).

4. **Restore WorldSave → GameWorld**
   - ✅ Implemented via `WorldLoadService` + `JsonFileStorage`.
   - Notes:
     - extra-tile data after the expected tile count is treated as an error.

5. **Integrate with runtime**
   - ✅ Load-or-generate on game initialization via `WorldPersistenceService` (generate-only for now).
   - ⬜ Use `saveFile` from config for load-or-generate when save/load is enabled.
   - ⬜ Add hotkeys:
     - `F5` save to `saveFile`
     - `F6` load from `saveFile`
   - ⬜ Implement safe world swap in `GameInterface` (update `GameArea` references).
   - ⬜ Ensure `saves/` directory exists when saving.

6. **Add format versioning strategy**
   - ✅ `saveVersion` required and validated on load.
   - ⬜ Add migration strategy (if/when v2 happens).

---

## Files Likely Affected

- `raylib-my/src/game_world.h`
- `raylib-my/src/game_world.cpp`
- `raylib-my/src/services/tiles_manager.h`
- `raylib-my/src/config/game_config.h`
- `raylib-my/src/config/game_config.cpp`
- `raylib-my/config/config.json`
- `raylib-my/src/common/keyboard_2d.h`
- `raylib-my/src/common/game_area.h`
- `raylib-my/src/game_interface.h`
- `raylib-my/src/game_interface.cpp`
- `raylib-my/src/world_tiles/tile.h`
- `raylib-my/src/world_tiles/resources/resource.h`
- `raylib-my/src/world_tiles/decorations/decoration.h`
- `raylib-my/src/world_persistence/world_data_reader.h` (new)
- `raylib-my/src/world_persistence/json_file_storage.h` (new)
- `raylib-my/src/world_persistence/json_file_storage.cpp` (new)
- `raylib-my/src/world_persistence/world_load_service.h` (new)
- `raylib-my/src/world_persistence/world_load_service.cpp` (new)
- `raylib-my/src/common/base64.h` (new)
- `raylib-my/src/common/base64.cpp` (new)
- `raylib-my/src/common/json_require.h` (new)

---

## Open Questions (Discuss Before Implementation)

1. Should “world save” include camera state (view), or only world simulation state?
2. What’s the canonical identifier for a tile type: terrain name, numeric ID, or something else?
3. Base64-in-JSON (single file) vs a sidecar binary file referenced by JSON (smaller + faster)?
4. Confirm `u16 little-endian` is OK for v1, and whether we should attempt `u8` when tileTypes count ≤ 256.
5. Do we need autosave / periodic saves, or only manual save/load for now?
6. For decorations: what does `decorationStates` represent for v1 (or should we omit it until decoration state exists)?
7. For resources: do we need to persist more than volume (e.g. initialVolume), or is `volume` enough for v1?
