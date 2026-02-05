#pragma once

#include "world_data_reader.h"

class WorldDataWriter {
public:
  virtual ~WorldDataWriter() = default;
  virtual void WriteMeta(const WorldMeta&) = 0;
  virtual void BeginTileWrite() = 0;
  virtual void WriteTile(const WorldTileData&) = 0;
  virtual void EndTileWrite() = 0;
};
