#ifndef NAMAGERS_TILES_MANAGER
#define NAMAGERS_TILES_MANAGER

#include <unordered_map>
#include "../world_tiles/tile.cpp"
#include "../world_tiles/tile_terrain_type.cpp"
#include "../graphics.cpp"

class TilesManager {
  public:
    TilesManager();
    void LoadTextures(Graphics&);
    WorldTile* NewTile(std::string, Position2D) const;
    std::vector<std::string> TileTypes() const;
    ~TilesManager();
    const std::unordered_map<std::string, WorldTileTerrainType> &TileTypes();
  private:
    const WorldTileTerrainType& Type(std::string) const;
    std::unordered_map<std::string, WorldTileTerrainType> tileTypes;
};

TilesManager::TilesManager() {
  std::string txr = "../../textures/terrain-1.png";
  std::vector<std::string> types { "Desert",  "Plains", "Grassland", "ForestBg", "HillsBg",
                                   "MountainsBg", "Tundra", "Arctic", "Swamp", "Jungle"};
  std::vector<float> mSpds { 0.8, 2, 1.5, 1.2, 1.3, 0.8, 2.0, 0.8, 0.5, 1.0 };

  for(int i = 0; i < types.size(); ++i) {
    Rectangle coords { 1.0f, 49.0f * i + 1.0f, 96.0f, 48.0f };
    tileTypes.emplace(types[i], WorldTileTerrainType { types[i], mSpds[i], false, txr, coords });
  }

  tileTypes.emplace("Deep Water", WorldTileTerrainType { "Deep Water", 0.5, true, "../../textures/ocean_sm.png", {} });
}

void TilesManager::LoadTextures(Graphics& grph) {
  for (auto& [name, tileType]: tileTypes) {
    tileType.LoadTexture(grph);
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
  for (auto &[name, tileType]: tileTypes) {
    UnloadTexture(tileType.Texture());
  }
}

#endif // NAMAGERS_TILES_MANAGER
