#include "simple_world_generator.h"

#include <limits>

#include "../common/game_error.h"

SimpleWorldGenerator::SimpleWorldGenerator(int world_width, int world_height):
  width { world_width },
  height { world_height },
  tileIndex { 0 },
  initialized { false },
  meta { }
{}

WorldMeta SimpleWorldGenerator::ReadMeta() {
  EnsureInitialized();
  return meta;
}

void SimpleWorldGenerator::BeginTileScan() {
  EnsureInitialized();
  tileIndex = 0;
}

std::optional<WorldTileData> SimpleWorldGenerator::NextTile() {
  EnsureInitialized();

  const size_t count = static_cast<size_t>(width) * static_cast<size_t>(height);
  if (tileIndex >= count) {
    return std::nullopt;
  }

  const int x = static_cast<int>(tileIndex % static_cast<size_t>(width));
  const int y = static_cast<int>(tileIndex / static_cast<size_t>(width));
  ++tileIndex;
  return BuildTile(x, y);
}

void SimpleWorldGenerator::EnsureInitialized() {
  if (initialized) return;

  if (width <= 0 || height <= 0) {
    throw GameError("SimpleWorldGenerator requires positive dimensions");
  }

  const size_t maxSize = std::numeric_limits<size_t>::max();
  const size_t widthSize = static_cast<size_t>(width);
  const size_t heightSize = static_cast<size_t>(height);
  if (widthSize > maxSize / heightSize) {
    throw GameError("SimpleWorldGenerator dimensions are too large to allocate tile grid");
  }

  meta = {};
  meta.width = width;
  meta.height = height;
  meta.tileTypeNamesById = { "Deep Water", "Plains", "Grassland" };
  meta.decorationNamesById = { "" };
  meta.resourceNamesById = { "" };

  initialized = true;
}

WorldTileData SimpleWorldGenerator::BuildTile(int x, int y) const {
  const int oceanWidth = 3;
  const int plainsWidth = 10;

  WorldTileData tile;
  if ((x < oceanWidth) || (y < oceanWidth) || (x > width - oceanWidth - 1) || (y > height - oceanWidth - 1)) {
    tile.tileTypeId = 0;
    return tile;
  }

  if ((x < plainsWidth) || (y < plainsWidth) || (x > width - plainsWidth - 1) || (y > height - plainsWidth - 1)) {
    tile.tileTypeId = 1;
    return tile;
  }

  tile.tileTypeId = 2;
  return tile;
}
