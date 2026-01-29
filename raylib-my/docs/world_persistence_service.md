## World Persistance System

### General Place in the System

```plantuml
@startuml
top to bottom direction

legend top right
What can be changed:
- Different generator types can be used
- Different file formats for world storage

endlegend

package "Menu" as MenuPackage #Violet {
  class Menu {
    - position: Rectangle2D
  }
  class DecorationMenu {

  }
  Menu <|-- DecorationMenu
}

package "Services" as ServicesPackage {
  class ServiceLocator {
    + Initialize(GameConfig config)
    + LoadResources()
    + GetTilesManager()
    + GetConfig()
    - tilesManager: TilesManager
    - gameConfig: GameConfig
  }
}

package GameObjects as  GameObjectsPackage <<GameObject>> #lightBlue {
  class GameInterface {
    + <b>constructor</b> GameInterface(int width, int height)
    - void AddArea(GameArea area)

    - gameWorld: GameWorld
    - currentMenu: Menu
    - gameAreas[]: GameArea
  }

  class GameArea {
    - gameObject: GameObject
    - position: Rectangle2D
  }

  GameInterface *--- GameArea

  note top of GameArea
    GameObject can be
    Menu or GameWorld
  end note
}

package "GameWorld" as GameWorldPackage #LightYellow {
  class GameWorld {
    {static} NewWorld(int width, int height): std::unique_ptr<GameWorld>
    {static} NewWorld(int width, int height, TilesProvider tiles): std::unique_ptr<GameWorld>
    - grid[][]: Tile
    - camera: GameCamera
  }
}

package "WorldPersistence" as WorldPersistencePackage #LightGreen {
  class WorldPersistenceService {
    Loads/saves world snapshots to/from file and
    provides startup load-or-generate behavior.
    ----
    + <b>constructor</b> WorldPersistenceService(GameConfig config)
    + LoadOrGenerate(): std::unique_ptr<GameWorld>
    + LoadWorld(): std::unique_ptr<GameWorld>
    + SaveWorld(const GameWorld& world): void
  }
}

ServiceLocator .. GameInterface: > config
ServiceLocator .. Menu: > config
ServiceLocator .. GameWorld: > config, \n tilesManager
ServiceLocator .. DecorationMenu: > tilesManager
ServiceLocator .. WorldPersistenceService: > config, \n tilesManager

Menu - GameInterface
GameInterface - GameWorld
GameInterface .. WorldPersistenceService: uses

Menu - GameArea
GameWorld - GameArea
@enduml
```

### World Persistence Design Rules

- `WorldDataReader`/`WorldDataWriter` must expose a format-neutral data contract (tile type maps + blobs + packed state arrays), not JSON/Base64 details.
- World packing rules (e.g., state arrays stored only for non-zero ids) must live in exactly one place per strategy:
  - file strategy: `JsonFileStorage`
  - generation strategy: `WorldGenerator`
- `WorldLoadService` must stay free of encoding/persistence knowledge: it only consumes `WorldDataReader` and produces a new `GameWorld`.
- `WorldPersistenceService` is the policy owner: decides which `WorldDataReader` strategy to use (load vs generate) and triggers save/load via hotkeys/startup.

### World persistence service details

```plantuml
@startuml
top to bottom direction

package "GameWorld" as GameWorldPackage #LightYellow {
  class GameWorld {
    {static} NewWorld(int width, int height): std::unique_ptr<GameWorld>
    {static} NewWorld(int width, int height, TilesProvider tiles): std::unique_ptr<GameWorld>
    - grid[][]: Tile
    - camera: GameCamera
  }
}

package "WorldPersistence" as WorldPersistencePackage #LightGreen {
  class WorldPersistenceService {
    Public API used by GameInterface.
    Returns a <b>new</b> GameWorld instance on load
    (runtime load replaces the current world).
    ----
    Uses `config.game.saveFile` as the target path.
    ----
    + <b>constructor</b> WorldPersistenceService(GameConfig config)
    + LoadOrGenerate(): std::unique_ptr<GameWorld>
    + LoadWorld(): std::unique_ptr<GameWorld>
    + SaveWorld(const GameWorld& world): void
  }

  interface WorldDataReader {
    Provides world data for loading.
    Different implementations:
    - file
    - generator
  }

  interface WorldDataWriter {
    Sink for saving world data.
    Typical implementation: file.
  }

  class WorldLoadService {
    Builds a new GameWorld instance using WorldDataReader.
    Responsible for world reconstruction (tiles/resources/decorations).
    ----
    + <b>constructor</b> WorldLoadService(const TilesManager& tiles, WorldDataReader& reader)
    + BuildWorld(): std::unique_ptr<GameWorld>
  }

  class WorldSaver {
    Extracts world data from GameWorld and writes it via WorldDataWriter.
  }

  class JsonFileStorage {
    Implements WorldDataReader/WorldDataWriter.
    Owns file IO + JSON/Base64 encode/decode + validation.
    Stores the in-memory save snapshot (type maps + blobs).
  }

  class WorldGenerator {
    Procedural world creation (seeded).
    Independent from file format and IO.
    ----
    Implements WorldDataReader for generated worlds.
  }

  class TilesManager {
    Creates tiles by terrain type name.
    ----
    + NewTile(std::string, Position2D): WorldTile*
  }

  WorldPersistenceService --> WorldLoadService
  WorldPersistenceService --> WorldSaver

  WorldLoadService ..> WorldDataReader
  TilesManager ..> WorldLoadService
  WorldSaver ..> WorldDataWriter

  WorldDataReader <|-- JsonFileStorage
  WorldDataWriter <|-- JsonFileStorage
  WorldDataReader <|-- WorldGenerator

  WorldPersistenceService .up.> GameWorld
  WorldSaver .left.> TilesManager

  note right of WorldPersistenceService
    Selects WorldDataReader strategy:
    - JsonFileStorage (load)
    - WorldGenerator (generate)
    and passes it (plus TilesManager) to WorldLoadService
  end note

  note bottom of WorldLoadService
    Provides GameWorld with a `TilesProvider` function
    which pulls tile data from WorldDataReader (stream)
    and uses TilesManager to create actual tile objects.
  end note
}

@enduml
```

### World Loader Interface

```plantuml
@startuml
top to bottom direction

class WorldLoadService {
  Builds a new GameWorld instance from WorldDataReader.
  WorldDataReader hides encoding/packing details.
  ----
  + <b>constructor</b> WorldLoadService(const TilesManager& tiles, WorldDataReader& reader)
  + BuildWorld(): std::unique_ptr<GameWorld>
  - ProvideTile(x: int, y: int): std::unique_ptr<WorldTile>
}

class WorldMeta {
  + width: int
  + height: int
  + tileTypeNamesById[]: string
  + decorationNamesById[]: string
  + resourceNamesById[]: string
  + camera: CameraState?
}

interface WorldDataReader {
  Format-neutral snapshot reader.
  Must provide a deterministic tile iteration order.
  ----
  + ReadMeta(): WorldMeta
  + BeginTileScan(): void
  + NextTile(): WorldTileData?
}

class CameraState {
  + offset: Position2D
  + target: Position2D
  + rotation: float
  + zoom: float
}

class WorldTileData {
  + tileTypeId: uint16
  + decorationTypeId: uint16
  + resourceTypeId: uint16
  + resourceVolume: uint32?  ' present only when resourceTypeId != 0
  + decorationState: uint32? ' present only when decorationTypeId != 0
}

class TilesManager {
  + NewTile(std::string, Position2D): WorldTile*
}

WorldLoadService ..> WorldDataReader
WorldLoadService .left.> TilesManager: instantiate \n tile

CameraState ..> WorldMeta
WorldDataReader .right.> WorldMeta
WorldDataReader .left.> WorldTileData

class JsonFileStorage {
  Implements WorldDataReader for JSON files.
  Encapsulates JSON/Base64 details and validates loaded data.
  ----
  + <b>constructor</b> JsonFileStorage(std::string path)
  + ReadMeta(): WorldMeta
  + BeginTileScan(): void
  + NextTile(): WorldTileData?
  ----
  - LoadFromFile(): void
  - LoadFromJson(const std::string&): void
  - ValidateLoadedData() const: void
  ----
  - path: std::string
  - initialized: bool
  - meta: WorldMeta
  - width: int
  - height: int
  - tiles: std::vector<uint16_t>
  - decorations: std::vector<uint16_t>
  - resources: std::vector<uint16_t>
  - resourceVolumesPacked: std::vector<uint32_t>
  - decorationStatesPacked: std::vector<uint32_t>
  - tileIndex: size_t
  - resourceVolumeIndex: size_t
  - decorationStateIndex: size_t
}
WorldDataReader <|.. JsonFileStorage

class "nlohmann::json" as NlohmannJson <<library>>
class Base64 <<codec>> {
  + Decode(const std::string& input): std::vector<uint8_t>
}
JsonFileStorage ..> NlohmannJson: parse/validate
JsonFileStorage ..> Base64: decode blobs

note left of JsonFileStorage
  Base64 + JSON libraries are implementation
  details of this reader.
  WorldLoadService depends only on
  WorldDataReader contract.
end note

class WorldGenerator
WorldDataReader <|.. WorldGenerator

@enduml
```

#### Contract Notes (for WorldDataReader)

- `ReadMeta()` must fully describe the world dimensions and provide id->name lookup arrays for tile/decor/resource ids.
- `BeginTileScan()` + `NextTile()` must produce exactly `width * height` tiles in <b>row-major</b> order (y=0..H-1, x=0..W-1).
- `NextTile()` must hide packing rules:
  - If `resourceTypeId == 0`, `resourceVolume` must be omitted/empty.
  - If `decorationTypeId == 0`, `decorationState` must be omitted/empty.
- Errors should be actionable (e.g., “unknown tileTypeId=7 at index 1234”).

### Startup (Load-or-Generate)

```plantuml
@startuml
title Startup: load world and build tiles via GameWorld::TilesProvider

actor "Game Startup" as Startup

participant WorldPersistenceService as Persist
participant JsonFileStorage as Storage
participant WorldLoadService as Loader
participant TilesManager as Tiles
participant "GameWorld" as World
participant "GameWorld::TilesProvider" as TilesProvider

Startup -> Persist: LoadOrGenerate()

Persist -> Storage: create(path)
Persist -> Loader: create(Tiles, Storage)
Persist -> Loader: BuildWorld()

Loader -> Storage: ReadMeta()
Loader -> Storage: BeginTileScan()

Loader -> World: NewWorld(width, height, TilesProvider)
activate World
World -> World: InitializeGrid(TilesProvider)
loop for each (x,y) in row-major
  World -> TilesProvider: (x,y)
  TilesProvider -> Loader: ProvideTile(x,y)
  Loader -> Storage: NextTile()
  Loader -> Tiles: NewTile(tileTypeName, pos)
  Loader --> TilesProvider: std::unique_ptr<WorldTile>
  TilesProvider --> World: std::unique_ptr<WorldTile>
end
deactivate World

Loader -> Storage: NextTile()
note right of Loader
  Must be empty/nullopt here.
  Extra data is an error.
end note

Loader --> Persist: std::unique_ptr<GameWorld>
Persist --> Startup: std::unique_ptr<GameWorld>
@enduml
```
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
