#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <memory>

#include "../common/game_error.h"
#include "../game_world.h"
#include "../services/tiles_manager.h"
#include "../world_tiles/decorations/decoration.h"
#include "../world_tiles/resources/resource.h"

#include "world_data_reader.h"

class WorldLoadService {
public:
  explicit WorldLoadService(const TilesManager& tiles_manager, WorldDataReader& reader);
  std::unique_ptr<GameWorld> BuildWorld();

private:
  WorldDataReader& reader;
  WorldMeta worldMeta;
  const TilesManager& tilesManager;

  std::unique_ptr<WorldTileDecoration> BuildDecoration(const std::string& decoration_name) const;
  std::unique_ptr<WorldTileResource> BuildResource(const std::string& resource_name, uint32_t volume) const;
  std::unique_ptr<WorldTile> ProvideTile(int x, int y) const;

  // TODO: This is poorly extendable in the case of adding new decorations and resource types
  const std::unordered_map<std::string_view, WorldDecorationType> decorationTypes = {
    {"Grass", WorldDecorationType::Grass},
    {"Rock", WorldDecorationType::Rock},
    {"Wall", WorldDecorationType::Wall},
    {"Tree", WorldDecorationType::Tree},
    {"Road", WorldDecorationType::Road},
  };

  const std::unordered_map<std::string_view, ResourceType> resourceTypes = {
    {"Coil", ResourceType::Coil},
    {"Clay", ResourceType::Clay},
    {"Iron", ResourceType::Iron},
    {"Copper", ResourceType::Copper},
  };
};
