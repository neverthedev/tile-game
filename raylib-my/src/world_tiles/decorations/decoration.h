#pragma once

#include <cstdint>
#include <string>

enum class WorldDecorationType { Grass, Rock, Wall, Tree, Road };

class WorldTileDecoration {
public:
  WorldDecorationType Type;

  WorldTileDecoration(WorldDecorationType, std::string);
  const std::string& Name() const;
  int MoveSpeed();
  uint32_t State() const;
  ~WorldTileDecoration();

private:
  std::string name;
};
