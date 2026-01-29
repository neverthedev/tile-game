#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

#include "../common/position_2d.h"

struct CameraState {
  Position2D offset { 0.0f, 0.0f };
  Position2D target { 0.0f, 0.0f };
  float rotation = 0.0f;
  float zoom = 1.0f;
};

struct WorldMeta {
  int width = 0;
  int height = 0;

  std::vector<std::string> tileTypeNamesById;
  std::vector<std::string> decorationNamesById;
  std::vector<std::string> resourceNamesById;

  std::optional<CameraState> camera;
};

struct WorldTileData {
  uint16_t tileTypeId = 0;
  uint16_t decorationTypeId = 0;
  uint16_t resourceTypeId = 0;
  std::optional<uint32_t> resourceVolume;
  std::optional<uint32_t> decorationState;
};

class WorldDataReader {
public:
  virtual ~WorldDataReader() = default;
  virtual WorldMeta ReadMeta() = 0;
  virtual void BeginTileScan() = 0;
  virtual std::optional<WorldTileData> NextTile() = 0;
};
