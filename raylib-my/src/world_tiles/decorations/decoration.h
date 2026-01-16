#pragma once

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
