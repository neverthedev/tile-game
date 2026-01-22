#pragma once

#include <memory>

class TilesManager;
class ResourcesSystem;
class GameConfig;

class ServiceLocator {
public:
  static void Initialize(const GameConfig& config);
  static void Shutdown();
  static void LoadResources(class ResourcesSystem& resources);
  static TilesManager& GetTilesManager();
  static const GameConfig& GetConfig();

private:
  static std::unique_ptr<TilesManager> tilesManager;
  static std::unique_ptr<GameConfig> gameConfig;
};
