#include "tiles_manager.h"

#include "../common/rectangle_2d.h"
#include "../graphics/resources_system.h"

TilesManager::TilesManager() {
  std::string txr = "../../textures/terrain-1.png";
  std::vector<std::string> types { "Desert",  "Plains", "Grassland", "ForestBg", "HillsBg",
                                   "MountainsBg", "Tundra", "Arctic", "Swamp", "Jungle"};
  std::vector<float> mSpds { 0.8, 2, 1.5, 1.2, 1.3, 0.8, 2.0, 0.8, 0.5, 1.0 };

  for(int i = 0; i < types.size(); ++i) {
    Rectangle2D coords { 1.0f, 49.0f * i + 1.0f, 96.0f, 48.0f };
    tileTypes.emplace(types[i], WorldTileTerrainType { types[i], mSpds[i], false, txr, coords });
  }

  tileTypes.emplace("Deep Water", WorldTileTerrainType { "Deep Water", 0.5, true, "../../textures/ocean_sm.png", Rectangle2D{0, 0, 0, 0} });
}

void TilesManager::LoadTextures(ResourcesSystem& resources) {
  for (auto& [name, tileType]: tileTypes) {
    tileType.LoadTexture(resources);
  }
}

std::vector<std::string> TilesManager::TileTypes() const {
  std::vector<std::string> keys;
  keys.reserve(tileTypes.size());
  for (const auto& [name, tileType] : tileTypes) {
    keys.push_back(name);
  }
  return keys;
}

WorldTile* TilesManager::NewTile(std::string typeName, Position2D pos) const {
  return Type(typeName).NewTile(pos);
}

const WorldTileTerrainType& TilesManager::Type(std::string typeName) const {
  return tileTypes.at(typeName);
}

TilesManager::~TilesManager() {
  // TODO: Need to pass Graphics/ResourcesSystem to properly unload textures
  // For now, textures will be unloaded when Graphics is destroyed
}
