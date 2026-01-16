#include "decoration.h"

WorldTileDecoration::WorldTileDecoration(WorldDecorationType type, std::string name):
  Type { type },
  Name { name }
{}

int WorldTileDecoration::MoveSpeed() {
  return 1;
}

WorldTileDecoration::~WorldTileDecoration() {}
