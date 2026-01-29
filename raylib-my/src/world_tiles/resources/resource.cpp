#include "resource.h"

WorldTileResource::WorldTileResource(ResourceType type, std::string name, uint32_t init_vol):
  Type { type },
  Name { name },
  volume { init_vol },
  initialVolume { init_vol }
{}

uint32_t WorldTileResource::Volume() {
  return volume;
}

WorldTileResource::~WorldTileResource() {}
