#include "service_locator.h"

#include "tiles_manager.h"
#include "../config/game_config.h"
#include "../graphics/resources_system.h"

#include <cassert>

std::unique_ptr<TilesManager> ServiceLocator::tilesManager = nullptr;
std::unique_ptr<GameConfig> ServiceLocator::gameConfig = nullptr;

void ServiceLocator::Initialize(const GameConfig& config) {
  tilesManager = std::make_unique<TilesManager>();
  gameConfig = std::make_unique<GameConfig>(config);
}

void ServiceLocator::Shutdown() {
  tilesManager.reset();
  gameConfig.reset();
}

void ServiceLocator::LoadResources(ResourcesSystem& resources) {
  assert(tilesManager != nullptr && "ServiceLocator not initialized!");
  tilesManager->LoadTextures(resources);
}

TilesManager& ServiceLocator::GetTilesManager() {
  assert(tilesManager != nullptr && "ServiceLocator not initialized! Call ServiceLocator::Initialize() first.");
  return *tilesManager;
}

const GameConfig& ServiceLocator::GetConfig() {
  assert(gameConfig != nullptr && "ServiceLocator config not initialized! Call ServiceLocator::Initialize() first.");
  return *gameConfig;
}
