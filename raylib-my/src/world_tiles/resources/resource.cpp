#include "resource.h"

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
