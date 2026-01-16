#ifndef WORLD_TILES_TILE_DECORATION
#define WORLD_TILES_TILE_DECORATION

#include <string>

enum class WorldDecorationType { Grass, Rock, Wall, Tree, Road };

class WorldTileDecoration {
  public:
    std::string Name;
    WorldDecorationType Type;
    WorldTileDecoration(WorldDecorationType, std::string);
    int MoveSpeed();
    ~WorldTileDecoration();
};

WorldTileDecoration::WorldTileDecoration(WorldDecorationType type, std::string name):
  Type { type },
  Name { name }
{}

int WorldTileDecoration::MoveSpeed() {
  return 1;
};

WorldTileDecoration::~WorldTileDecoration() {};

#endif // WORLD_TILES_TILE_DECORATION
