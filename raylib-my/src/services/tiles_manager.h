#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "../world_tiles/tile.h"
#include "../world_tiles/tile_terrain_type.h"
#include "../world_tiles/decorations/decoration.h"
#include "../world_tiles/resources/resource.h"

// Forward declaration
class ResourcesSystem;

class TilesManager {
public:
  TilesManager();
  void LoadTextures(ResourcesSystem& resources);
  WorldTile* NewTile(std::string, Position2D) const;
  std::vector<std::string> TileTypeNames() const;
  WorldDecorationType DecorationTypeByName(const std::string&) const;
  ResourceType ResourceTypeByName(const std::string&) const;
  ~TilesManager();
  const std::unordered_map<std::string, WorldTileTerrainType> &TileTypes();

private:
  const WorldTileTerrainType& Type(std::string) const;
  std::unordered_map<std::string, WorldTileTerrainType> tileTypes;
  std::unordered_map<std::string, WorldDecorationType> decorationTypes;
  std::unordered_map<std::string, ResourceType> resourceTypes;
};
