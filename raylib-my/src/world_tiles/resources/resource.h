#pragma once

#include <cstdint>
#include <string>

enum class ResourceType { Coil, Clay, Iron, Copper };

class WorldTileResource {
public:
  ResourceType Type;

  WorldTileResource(ResourceType, std::string, uint32_t);
  const std::string& Name() const;
  uint32_t Volume() const;
  ~WorldTileResource();

private:
  std::string name;
  uint32_t volume;
  uint32_t initialVolume;
};
