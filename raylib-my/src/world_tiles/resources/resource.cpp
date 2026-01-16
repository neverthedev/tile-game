#ifndef WORLD_TILES_TILE_RESOURCE
#define WORLD_TILES_TILE_RESOURCE

#include <string>

enum class ResourceType { Coil, Clay, Iron, Copper };

class WorldTileResource {
  public:
    std::string Name;
    ResourceType Type;

    WorldTileResource(ResourceType, std::string, int);
    int Volume();
    ~WorldTileResource();
  private:
    int volume;
    int initialVolume;
};

WorldTileResource::WorldTileResource(ResourceType type, std::string name, int init_vol):
  Type { type },
  Name { name },
  volume { init_vol },
  initialVolume { init_vol }
{}

int WorldTileResource::Volume() {
  return volume;
}

WorldTileResource::~WorldTileResource() {}

#endif // WORLD_TILES_TILE_RESOURCE
