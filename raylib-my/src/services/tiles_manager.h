#pragma once

#include <unordered_map>
#include <vector>
#include <string>

#include "../world_tiles/tile.h"
#include "../world_tiles/tile_terrain_type.h"

// Forward declaration
class ResourcesSystem;

class TilesManager {
public:
  TilesManager();
  void LoadTextures(ResourcesSystem& resources);
  WorldTile* NewTile(std::string, Position2D) const;
  std::vector<std::string> TileTypes() const;
  ~TilesManager();
  const std::unordered_map<std::string, WorldTileTerrainType> &TileTypes();

private:
  const WorldTileTerrainType& Type(std::string) const;
  std::unordered_map<std::string, WorldTileTerrainType> tileTypes;
};
