#pragma once

#include <memory>
#include <functional>
#include <string>

#include "../common/game_error.h"
#include "../game_world.h"
#include "../services/tiles_manager.h"
#include "../world_tiles/decorations/decoration.h"
#include "../world_tiles/resources/resource.h"

#include "world_data_reader.h"

class WorldLoadService {
public:
  using WorldBuilder = std::function<std::unique_ptr<GameWorld>(int, int, GameWorld::TileProvider)>;

  WorldLoadService(const TilesManager& tiles_manager, WorldDataReader& reader, WorldBuilder world_builder);
  std::unique_ptr<GameWorld> BuildWorld();

private:
  WorldDataReader& reader;
  WorldMeta worldMeta;
  const TilesManager& tilesManager;
  WorldBuilder worldBuilder;

  std::unique_ptr<WorldTileDecoration> BuildDecoration(const std::string& decoration_name) const;
  std::unique_ptr<WorldTileResource> BuildResource(const std::string& resource_name, uint32_t volume) const;
  std::unique_ptr<WorldTile> ProvideTile(int x, int y) const;
};
