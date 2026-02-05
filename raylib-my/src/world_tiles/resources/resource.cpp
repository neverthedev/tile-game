#include "resource.h"

WorldTileResource::WorldTileResource(ResourceType type, std::string name, uint32_t init_vol):
  Type { type },
  name { name },
  volume { init_vol },
  initialVolume { init_vol }
{}

const std::string& WorldTileResource::Name() const {
  return name;
}

uint32_t WorldTileResource::Volume() const {
  return volume;
}

WorldTileResource::~WorldTileResource() {}
