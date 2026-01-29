#include "tiles_manager.h"

#include "../common/rectangle_2d.h"
#include "../common/game_error.h"
#include "../graphics/resources_system.h"

TilesManager::TilesManager() {
  std::string txr = "../../textures/terrain-1.png";
  std::vector<std::string> types { "Desert",  "Plains", "Grassland", "ForestBg", "HillsBg",
                                   "MountainsBg", "Tundra", "Arctic", "Swamp", "Jungle"};
  std::vector<float> mSpds { 0.8, 2, 1.5, 1.2, 1.3, 0.8, 2.0, 0.8, 0.5, 1.0 };

  for(int i = 0; i < types.size(); ++i) {
    Rectangle2D coords { 1.0f, 49.0f * i + 1.0f, 96.0f, 48.0f };
    tileTypes.try_emplace(types[i],  types[i], mSpds[i], false, txr, coords);
  }

  tileTypes.try_emplace("Deep Water", "Deep Water", 0.5, true, "../../textures/ocean_sm.png", Rectangle2D{0, 0, 0, 0});

  decorationTypes = {
    { "Grass", WorldDecorationType::Grass },
    { "Rock", WorldDecorationType::Rock },
    { "Wall", WorldDecorationType::Wall },
    { "Tree", WorldDecorationType::Tree },
    { "Road", WorldDecorationType::Road }
  };

  resourceTypes = {
    { "Coil", ResourceType::Coil },
    { "Clay", ResourceType::Clay },
    { "Iron", ResourceType::Iron },
    { "Copper", ResourceType::Copper }
  };
}

void TilesManager::LoadTextures(ResourcesSystem& resources) {
  for (auto& [name, tileType]: tileTypes) {
    tileType.LoadTexture(resources);
  }
}

std::vector<std::string> TilesManager::TileTypeNames() const {
  std::vector<std::string> keys;
  keys.reserve(tileTypes.size());
  for (const auto& [name, tileType] : tileTypes) {
    keys.push_back(name);
  }
  return keys;
}

const std::unordered_map<std::string, WorldTileTerrainType> &TilesManager::TileTypes() {
  return tileTypes;
}

WorldTile* TilesManager::NewTile(std::string typeName, Position2D pos) const {
  return Type(typeName).NewTile(pos);
}

WorldDecorationType TilesManager::DecorationTypeByName(const std::string& name) const {
  auto it = decorationTypes.find(name);
  if (it == decorationTypes.end()) {
    throw GameError("Unknown decoration type name: " + name);
  }
  return it->second;
}

ResourceType TilesManager::ResourceTypeByName(const std::string& name) const {
  auto it = resourceTypes.find(name);
  if (it == resourceTypes.end()) {
    throw GameError("Unknown resource type name: " + name);
  }
  return it->second;
}

const WorldTileTerrainType& TilesManager::Type(std::string typeName) const {
  return tileTypes.at(typeName);
}

TilesManager::~TilesManager() {
  // TODO: Need to pass Graphics/ResourcesSystem to properly unload textures
  // For now, textures will be unloaded when Graphics is destroyed
}
