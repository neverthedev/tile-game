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

## Current Progress (as of February 4, 2026)

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

✅ Implemented (saving pipeline)
- `WorldDataWriter` contract.
- `WorldSaveService` that extracts `GameWorld` into the format-neutral snapshot and streams it to `WorldDataWriter`.
- `JsonFileStorage` writer side:
  - validates `WriteMeta()`/`BeginTileWrite()`/`WriteTile()`/`EndTileWrite()` contract,
  - validates id/state presence consistency,
  - serializes packed arrays to Base64 and writes JSON.
- Base64 encode support in `src/common/base64.*`.
- Stable save-side identifiers:
  - `WorldTileTerrainType::Name()`,
  - `WorldTile::TerrainTypeName()`,
  - `WorldTile::Decoration()` / `WorldTile::Resource()` (non-owning const pointers),
  - `WorldTileDecoration::Name()` / `State()`,
  - `WorldTileResource::Name()` / `Volume()`.
- Save path in config:
  - `game.saveFile` field added to `GameConfig` and `config/config.json`.
- `WorldPersistenceService::SaveWorld()` is wired through `WorldSaveService` + `JsonFileStorage` using `config.SaveFile`.

🚧 Not implemented yet (still required for this task)
- Runtime integration:
  - `LoadWorld()` is still disabled; `LoadOrGenerate()` still generates only.
  - `F5` save, `F6` load (and world swap in `GameInterface` without dangling `GameArea` references)

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
  - ✅ `WorldDataWriter` interface and writer side in `JsonFileStorage`.
  - ✅ Base64 encode.

2. **Implement generation + startup policy (load-or-generate)**
   - ✅ Create `SimpleWorldGenerator` that implements `WorldDataReader` for procedural worlds.
   - ✅ Move current procedural logic from `GameWorld` factory into `SimpleWorldGenerator`.
   - ✅ Create `WorldPersistenceService` and make startup use it:
     - `LoadWorld()` will use `JsonFileStorage` + `WorldLoadService` (pending).
     - `GenerateWorld()` uses `SimpleWorldGenerator` + `WorldLoadService`.
     - `LoadOrGenerate()` currently generates only (load-first policy pending).
     - `SaveWorld()` is implemented via `WorldSaveService`.

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
     - camera state
   - ✅ Implemented.

4. **Restore WorldSave → GameWorld**
   - ✅ Implemented via `WorldLoadService` + `JsonFileStorage`.
   - Notes:
     - extra-tile data after the expected tile count is treated as an error.

5. **Integrate with runtime**
   - ✅ Load-or-generate on game initialization via `WorldPersistenceService` (generate-only for now).
   - ✅ Add `saveFile` in config and use it for `SaveWorld`.
   - ⬜ Use `saveFile` in `LoadWorld` / load-first startup path.
   - ⬜ Add hotkeys:
     - `F5` save to `saveFile`
     - `F6` load from `saveFile`
   - ⬜ Implement safe world swap in `GameInterface` (update `GameArea` references).
   - ⬜ Ensure save directory exists when saving.

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
- `raylib-my/src/world_persistence/world_data_writer.h` (new)
- `raylib-my/src/world_persistence/json_file_storage.h` (new)
- `raylib-my/src/world_persistence/json_file_storage.cpp` (new)
- `raylib-my/src/world_persistence/world_load_service.h` (new)
- `raylib-my/src/world_persistence/world_load_service.cpp` (new)
- `raylib-my/src/world_persistence/world_save_service.h` (new)
- `raylib-my/src/world_persistence/world_save_service.cpp` (new)
- `raylib-my/src/common/base64.h` (new)
- `raylib-my/src/common/base64.cpp` (new)
- `raylib-my/src/common/json_require.h` (new)
- `raylib-my/src/world_tiles/tile_terrain_type.h`
- `raylib-my/src/world_tiles/tile_terrain_type.cpp`

---

## Open Questions (Remaining)

1. Do we need autosave / periodic saves, or only manual save/load for now?
2. Should we keep JSON+Base64 only for v1, or add optional compression in this task?
3. How should migration be handled once `saveVersion` changes (v2+)?
