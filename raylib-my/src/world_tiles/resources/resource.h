#pragma once

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
