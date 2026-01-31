# Service Locator Pattern

## Overview

The Service Locator pattern provides centralized access to application-wide resources and services. It decouples resource consumers from resource providers, eliminating the need to pass resources through constructor chains.

## Architecture

### Package Dependencies

```plantuml
@startuml
package "services" {
  class ServiceLocator {
    + {static} Initialize(GameConfig config)
    + {static} Shutdown()
    + {static} LoadResources(Graphics&)
    + {static} GetTilesManager() : TilesManager&
    + {static} GetConfig() : GameConfig&
    - {static} tilesManager : unique_ptr<TilesManager>
    - {static} gameConfig : unique_ptr<GameConfig>
  }

  class TilesManager {
    + LoadTextures(Graphics&)
    + NewTile(string, Position2D) : WorldTile*
    + TileTypes() : vector<string>
    - tileTypes : map<string, WorldTileTerrainType>
  }
}

package "game" {
  class Game
  class GameInterface
  class GameWorld
  class WorldPersistenceService
  class DecorationMenu
}

package "tiles" {
  class WorldTile
  class WorldTileTerrainType
}
package "config" {
  class GameConfig
}

ServiceLocator --> TilesManager : owns
ServiceLocator --> GameConfig : owns
GameWorld ..> ServiceLocator : uses
Game ..> ServiceLocator : initializes
WorldPersistenceService ..> ServiceLocator : uses

TilesManager --> WorldTileTerrainType : manages
TilesManager ..> WorldTile : creates
GameWorld --> WorldTile : owns many

note right of ServiceLocator
  Global access point
  for application resources
end note

note right of TilesManager
  Tile type registry
  and factory
end note
@enduml
```

### Class Relationships

```plantuml
@startuml
class ServiceLocator <<singleton>> {
  + {static} Initialize(GameConfig config)
  + {static} Shutdown()
  + {static} LoadResources(Graphics&)
  + {static} GetTilesManager() : TilesManager&
  + {static} GetConfig() : GameConfig&
  - {static} tilesManager : unique_ptr<TilesManager>
  - {static} gameConfig : unique_ptr<GameConfig>
}

class TilesManager {
  + TilesManager()
  + LoadTextures(Graphics&)
  + NewTile(string, Position2D) : WorldTile*
  + TileTypes() : vector<string>
  - Type(string) : WorldTileTerrainType&
  - tileTypes : map<string, WorldTileTerrainType>
}

class GameWorld {
  - InitializeGrid()
  - grid : vector<unique_ptr<WorldTile>>
}

class WorldPersistenceService {
  + {static} CreateFromServices(): WorldPersistenceService
  + LoadOrGenerate(): std::unique_ptr<GameWorld>
}

class GameConfig

class Graphics {
  + LoadTexture(string)
}

ServiceLocator o-- TilesManager : owns
ServiceLocator o-- GameConfig : owns
GameWorld .> ServiceLocator : <<uses>>
TilesManager .> Graphics : <<uses>>
GameWorld --> WorldTile : creates via TilesManager
WorldPersistenceService .> ServiceLocator : <<uses>>
@enduml
```

## Initialization Sequence

```plantuml
@startuml
actor Main
participant ServiceLocator
participant TilesManager
participant Graphics
participant GameInterface
participant GameWorld
participant WorldPersistenceService
participant GameConfig

Main -> GameConfig : LoadFromFile()
Main -> ServiceLocator : Initialize(config)
activate ServiceLocator
ServiceLocator -> TilesManager : new TilesManager()
activate TilesManager
TilesManager -> TilesManager : Register tile types
deactivate TilesManager
ServiceLocator -> ServiceLocator : Store in static member
deactivate ServiceLocator

Main -> Graphics : new Graphics()
Main -> Graphics : InitScreen()

Main -> ServiceLocator : LoadResources(graphics)
activate ServiceLocator
ServiceLocator -> TilesManager : LoadTextures(graphics)
activate TilesManager
loop For each tile type
  TilesManager -> Graphics : LoadTexture(path)
end
deactivate TilesManager
deactivate ServiceLocator

Main -> GameInterface : new GameInterface()
activate GameInterface
GameInterface -> WorldPersistenceService : CreateFromServices()
activate WorldPersistenceService
WorldPersistenceService --> GameInterface : WorldPersistenceService
deactivate WorldPersistenceService
GameInterface -> WorldPersistenceService : LoadOrGenerate()
activate WorldPersistenceService
WorldPersistenceService -> WorldLoadService : BuildWorld()
activate WorldLoadService
WorldLoadService -> GameWorld : new GameWorld(width, height, tilesProvider)
activate GameWorld
GameWorld -> GameWorld : InitializeGrid()
GameWorld -> ServiceLocator : GetTilesManager()
activate ServiceLocator
ServiceLocator --> GameWorld : TilesManager&
deactivate ServiceLocator
loop For each tile position
  GameWorld -> TilesManager : NewTile(type, position)
  activate TilesManager
  TilesManager --> GameWorld : WorldTile*
  deactivate TilesManager
end
deactivate GameWorld
deactivate WorldLoadService
deactivate WorldPersistenceService
deactivate GameInterface

note right of Main
  Initialization order:
  1. ServiceLocator
  2. Graphics system
  3. Load resources
  4. Create game objects
end note
@enduml
```

## Runtime Usage

```plantuml
@startuml
participant GameWorld
participant ServiceLocator
participant TilesManager
participant WorldTile

GameWorld -> GameWorld : InitializeGrid()
activate GameWorld

GameWorld -> ServiceLocator : GetTilesManager()
activate ServiceLocator
ServiceLocator --> GameWorld : TilesManager&
deactivate ServiceLocator

note right of GameWorld
  Single call to get reference,
  then use directly
end note

loop For each grid position
  GameWorld -> TilesManager : NewTile("Deep Water", pos)
  activate TilesManager
  TilesManager -> WorldTile : new WorldTile(...)
  activate WorldTile
  WorldTile --> TilesManager : WorldTile*
  deactivate WorldTile
  TilesManager --> GameWorld : WorldTile*
  deactivate TilesManager

  GameWorld -> GameWorld : grid.push_back(tile)
end

deactivate GameWorld
@enduml
```

## Shutdown Sequence

```plantuml
@startuml
actor Main
participant GameInterface
participant GameWorld
participant ServiceLocator
participant TilesManager

Main -> GameInterface : ~GameInterface()
activate GameInterface
GameInterface -> GameWorld : ~GameWorld()
activate GameWorld
GameWorld -> GameWorld : Clear grid
note right: WorldTiles destroyed automatically
deactivate GameWorld
deactivate GameInterface

Main -> ServiceLocator : Shutdown()
activate ServiceLocator
ServiceLocator -> TilesManager : ~TilesManager()
activate TilesManager
TilesManager -> TilesManager : UnloadTexture() for each
deactivate TilesManager
ServiceLocator -> ServiceLocator : tilesManager.reset()
deactivate ServiceLocator

note right of Main
  Shutdown order:
  1. Game objects
  2. ServiceLocator
  3. Graphics system (implicit)
end note
@enduml
```

## Usage Examples

### Getting TilesManager

```cpp
auto persistence = WorldPersistenceService::CreateFromServices();
auto world = persistence.LoadOrGenerate();
```

### Initialization in Main

```cpp
int main() {
  auto config = GameConfig::LoadFromFile("config/config.json");
  ServiceLocator::Initialize(config);

  Graphics graphics{1024, 768, 64, 32, "Game", 60};
  graphics.InitScreen();

  ServiceLocator::LoadResources(graphics);

  GameInterface interface{1024, 768};

  while (!graphics.Done()) {
    interface.HandleInput();
    interface.Update();
    interface.Render(graphics);
  }

  ServiceLocator::Shutdown();
  return 0;
}
```

## Benefits

### Decoupling
- GameWorld doesn't depend on GameInterface to access TilesManager
- No need to pass TilesManager through constructor chains
- Easy to test components in isolation

### Centralized Lifecycle
- Single initialization point for all services
- Clear shutdown order
- Resources loaded once, used everywhere

### Scalability
- Easy to add new services (AudioManager, ConfigManager, etc.)
- No need to modify existing constructors
- Services can be mocked for testing

## Design Decisions

### Why Static Methods?

Global access needed for truly application-wide resources. Alternative would be passing ServiceLocator everywhere, which is more verbose.

### Why unique_ptr?

Clear ownership - ServiceLocator owns the TilesManager lifetime. Automatic cleanup in Shutdown().

### Why Not Singleton?

ServiceLocator allows controlled initialization/shutdown and can be replaced with mock implementations for testing. Pure singleton would be harder to test.

### Why TilesManager as Service?

TilesManager is:
- Application-level resource (not per-scene)
- Immutable after initialization
- Needed by multiple disconnected systems
- Resource registry + factory

This makes it ideal for ServiceLocator management.

## Adding New Services

To add a new service to ServiceLocator:

1. Add member variable:
```cpp
class ServiceLocator {
private:
  static std::unique_ptr<TilesManager> tilesManager;
  static std::unique_ptr<AudioManager> audioManager;  // New
};
```

2. Initialize in Initialize():
```cpp
void ServiceLocator::Initialize() {
  tilesManager = std::make_unique<TilesManager>();
  audioManager = std::make_unique<AudioManager>();  // New
}
```

3. Add getter:
```cpp
static AudioManager& GetAudioManager();
```

4. Implement getter:
```cpp
AudioManager& ServiceLocator::GetAudioManager() {
  assert(audioManager != nullptr);
  return *audioManager;
}
```

5. Clean up in Shutdown():
```cpp
void ServiceLocator::Shutdown() {
  audioManager.reset();  // New - order matters!
  tilesManager.reset();
}
```

## Files

- [services/service_locator.h](../src/services/service_locator.h)
- [services/service_locator.cpp](../src/services/service_locator.cpp)
- [services/tiles_manager.h](../src/services/tiles_manager.h)
- [services/tiles_manager.cpp](../src/services/tiles_manager.cpp)
