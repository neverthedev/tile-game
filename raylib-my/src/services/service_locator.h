#pragma once

#include <memory>

class TilesManager;
class ResourcesSystem;

class ServiceLocator {
public:
  static void Initialize();
  static void Shutdown();
  static void LoadResources(class ResourcesSystem& resources);
  static TilesManager& GetTilesManager();

private:
  static std::unique_ptr<TilesManager> tilesManager;
};
