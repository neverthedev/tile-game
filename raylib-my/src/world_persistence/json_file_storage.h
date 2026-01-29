#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "world_data_reader.h"

class JsonFileStorage final : public WorldDataReader {
 public:
  explicit JsonFileStorage(std::string);
  ~JsonFileStorage() override;

  JsonFileStorage(const JsonFileStorage&) = delete;
  JsonFileStorage& operator=(const JsonFileStorage&) = delete;
  JsonFileStorage(JsonFileStorage&&) = delete;
  JsonFileStorage& operator=(JsonFileStorage&&) = delete;

  WorldMeta ReadMeta() override;
  void BeginTileScan() override;
  std::optional<WorldTileData> NextTile() override;

 private:
  void LoadFromFile();
  void LoadFromJson(const std::string&);
  void ValidateLoadedData() const;

  std::string path;
  bool initialized;
  WorldMeta meta;
  int width;
  int height;

  std::vector<uint16_t> tiles;
  std::vector<uint16_t> decorations;
  std::vector<uint16_t> resources;
  std::vector<uint32_t> resourceVolumesPacked;
  std::vector<uint32_t> decorationStatesPacked;

  size_t tileIndex;
  size_t resourceVolumeIndex;
  size_t decorationStateIndex;
};
