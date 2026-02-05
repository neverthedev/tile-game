#include "decoration.h"

WorldTileDecoration::WorldTileDecoration(WorldDecorationType type, std::string name):
  Type { type },
  name { name }
{}

const std::string& WorldTileDecoration::Name() const {
  return name;
}

int WorldTileDecoration::MoveSpeed() {
  return 1;
}

uint32_t WorldTileDecoration::State() const {
  return 0;
}

WorldTileDecoration::~WorldTileDecoration() {}
