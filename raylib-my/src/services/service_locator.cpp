#include "service_locator.h"

#include "tiles_manager.h"
#include "../graphics.h"

#include <cassert>

std::unique_ptr<TilesManager> ServiceLocator::tilesManager = nullptr;

void ServiceLocator::Initialize() {
  tilesManager = std::make_unique<TilesManager>();
}

void ServiceLocator::Shutdown() {
  tilesManager.reset();
}

void ServiceLocator::LoadResources(Graphics& graphics) {
  assert(tilesManager != nullptr && "ServiceLocator not initialized!");
  tilesManager->LoadTextures(graphics);
}

TilesManager& ServiceLocator::GetTilesManager() {
  assert(tilesManager != nullptr && "ServiceLocator not initialized! Call ServiceLocator::Initialize() first.");
  return *tilesManager;
}
