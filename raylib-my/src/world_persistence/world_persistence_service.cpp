#include "world_persistence_service.h"

#include "../common/game_error.h"
#include "../config/game_config.h"
#include "../services/tiles_manager.h"
#include "../input_components/world_component.h"
#include "../graphics_components/world_component.h"
#include "../update_components/world_component.h"
#include "../services/service_locator.h"

#include "simple_world_generator.h"
#include "world_load_service.h"

WorldPersistenceService::WorldPersistenceService(const GameConfig& cfg, const TilesManager& tilesMngr):
  config { cfg },
  tilesManager { tilesMngr }
{}

WorldPersistenceService WorldPersistenceService::CreateFromServices() {
  return WorldPersistenceService(ServiceLocator::GetConfig(), ServiceLocator::GetTilesManager());
}

std::unique_ptr<GameWorld> WorldPersistenceService::LoadOrGenerate() {
  return GenerateWorld();
}

std::unique_ptr<GameWorld> WorldPersistenceService::LoadWorld() {
  throw GameError("World load is not available yet. Save/load pipeline is still in progress.");
}

std::unique_ptr<GameWorld> WorldPersistenceService::GenerateWorld() {
  SimpleWorldGenerator generator { config.WorldWidth, config.WorldHeight };
  WorldLoadService loader { tilesManager, generator, BuildWorldWithTiles };
  return loader.BuildWorld();
}

void WorldPersistenceService::SaveWorld(const GameWorld&) {
  throw GameError("World save is not available yet. Save/load pipeline is still in progress.");
}

std::unique_ptr<GameWorld> WorldPersistenceService::BuildWorldWithTiles(
  int width,
  int height,
  GameWorld::TileProvider tilesProvider
) {
  return std::make_unique<GameWorld>(
    width, height,
    std::make_unique<WorldInputComponent>(),
    std::make_unique<WorldGraphicsComponent>(),
    std::make_unique<WorldUpdateComponent>(),
    std::move(tilesProvider)
  );
}
