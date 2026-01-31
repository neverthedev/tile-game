#pragma once

#include <cstddef>
#include <optional>

#include "world_data_reader.h"

class SimpleWorldGenerator final : public WorldDataReader {
public:
  SimpleWorldGenerator(int width, int height);

  WorldMeta ReadMeta() override;
  void BeginTileScan() override;
  std::optional<WorldTileData> NextTile() override;

private:
  int width;
  int height;
  size_t tileIndex;
  bool initialized;
  WorldMeta meta;

  void EnsureInitialized();
  WorldTileData BuildTile(int x, int y) const;
};
