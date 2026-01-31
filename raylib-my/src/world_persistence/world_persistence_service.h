#pragma once

#include <memory>

#include "../game_world.h"

class GameConfig;
class TilesManager;

class WorldPersistenceService {
public:
  WorldPersistenceService(const GameConfig& config, const TilesManager& tilesManager);
  static WorldPersistenceService CreateFromServices();

  std::unique_ptr<GameWorld> LoadOrGenerate();
  std::unique_ptr<GameWorld> LoadWorld();
  std::unique_ptr<GameWorld> GenerateWorld();
  void SaveWorld(const GameWorld& world);

private:
  const GameConfig& config;
  const TilesManager& tilesManager;

  static std::unique_ptr<GameWorld> BuildWorldWithTiles(int width, int height, GameWorld::TileProvider tilesProvider);
};
