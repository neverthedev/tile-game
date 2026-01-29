#pragma once

#include <cstdint>
#include <string>

enum class ResourceType { Coil, Clay, Iron, Copper };

class WorldTileResource {
public:
  std::string Name;
  ResourceType Type;

  WorldTileResource(ResourceType, std::string, uint32_t);
  uint32_t Volume();
  ~WorldTileResource();

private:
  uint32_t volume;
  uint32_t initialVolume;
};
