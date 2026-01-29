#include "json_file_storage.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "../common/base64.h"
#include "../common/game_error.h"
#include "../common/json_require.h"

using nlohmann::json;

namespace {
  void ThrowGameError(const std::string& msg) {
    throw GameError(msg);
  }

  uint16_t ReadU16LE(const uint8_t* p) {
    return static_cast<uint16_t>(p[0]) | (static_cast<uint16_t>(p[1]) << 8);
  }

  uint32_t ReadU32LE(const uint8_t* p) {
    return static_cast<uint32_t>(p[0])
      | (static_cast<uint32_t>(p[1]) << 8)
      | (static_cast<uint32_t>(p[2]) << 16)
      | (static_cast<uint32_t>(p[3]) << 24);
  }

  std::vector<std::string> BuildNamesById(const json& j_obj, const char* name) {
    const json& typeMap = JsonRequire::Object(j_obj, name, ThrowGameError);

    int maxId = -1;
    std::unordered_map<int, std::string> temp;
    for (auto it = typeMap.begin(); it != typeMap.end(); ++it) {
      const std::string key = it.key();
      if (!it.value().is_number_integer()) {
        throw GameError(std::string("Invalid type id for '") + key + "' in " + name);
      }

      int id = it.value().get<int>();
      if (id < 0) {
        throw GameError(std::string("Negative type id for '") + key + "' in " + name);
      }
      if (temp.find(id) != temp.end()) {
        throw GameError(std::string("Duplicate type id ") + std::to_string(id) + " in " + name);
      }

      temp.emplace(id, key);
      maxId = std::max(maxId, id);
    }

    if (maxId < 0) {
      throw GameError(std::string("Empty type map: ") + name);
    }

    std::vector<std::string> out(static_cast<size_t>(maxId) + 1, "");
    for (const auto& [id, typeName] : temp) {
      out[static_cast<size_t>(id)] = typeName;
    }
    return out;
  }

  std::vector<uint16_t> DecodeU16Array(const std::string& b64, size_t expected_count, const char* label) {
    std::vector<uint8_t> bytes = Base64::Decode(b64);
    const size_t expected_bytes = expected_count * 2;
    if (bytes.size() != expected_bytes) {
      throw GameError(std::string("Invalid byte length for ") + label + ": expected "
        + std::to_string(expected_bytes) + ", got " + std::to_string(bytes.size()));
    }

    std::vector<uint16_t> out(expected_count);
    for (size_t i = 0; i < expected_count; ++i) {
      out[i] = ReadU16LE(&bytes[i * 2]);
    }
    return out;
  }

  std::vector<uint32_t> DecodeU32Array(const std::string& b64, const char* label) {
    std::vector<uint8_t> bytes = Base64::Decode(b64);
    if (bytes.size() % 4 != 0) {
      throw GameError(std::string("Invalid byte length for ") + label + ": expected multiple of 4, got "
        + std::to_string(bytes.size()));
    }

    const size_t count = bytes.size() / 4;
    std::vector<uint32_t> out(count);
    for (size_t i = 0; i < count; ++i) {
      out[i] = ReadU32LE(&bytes[i * 4]);
    }
    return out;
  }

  void ValidateEncoding(const json& world) {
    const json& enc = JsonRequire::Object(world, "encoding", ThrowGameError);
    const std::string order = JsonRequire::Field<std::string>(enc, "order", ThrowGameError);
    const std::string valueType = JsonRequire::Field<std::string>(enc, "valueType", ThrowGameError);
    const std::string endianness = JsonRequire::Field<std::string>(enc, "endianness", ThrowGameError);
    const std::string codec = JsonRequire::Field<std::string>(enc, "codec", ThrowGameError);

    if (order != "row-major") {
      throw GameError("Unsupported encoding.order: " + order);
    }
    if (valueType != "u16") {
      throw GameError("Unsupported encoding.valueType: " + valueType);
    }
    if (endianness != "little") {
      throw GameError("Unsupported encoding.endianness: " + endianness);
    }
    if (codec != "base64") {
      throw GameError("Unsupported encoding.codec: " + codec);
    }
  }
}

JsonFileStorage::JsonFileStorage(std::string path):
  path { std::move(path) },
  initialized { false },
  meta { },
  width { 0 },
  height { 0 },
  tiles { },
  decorations { },
  resources { },
  resourceVolumesPacked { },
  decorationStatesPacked { },
  tileIndex { 0 },
  resourceVolumeIndex { 0 },
  decorationStateIndex { 0 }
{}

JsonFileStorage::~JsonFileStorage() = default;

WorldMeta JsonFileStorage::ReadMeta() {
  LoadFromFile();
  return meta;
}

void JsonFileStorage::BeginTileScan() {
  LoadFromFile();
  tileIndex = 0;
  resourceVolumeIndex = 0;
  decorationStateIndex = 0;
}

std::optional<WorldTileData> JsonFileStorage::NextTile() {
  LoadFromFile();

  const size_t count = static_cast<size_t>(width) * static_cast<size_t>(height);
  if (tileIndex >= count) {
    return std::nullopt;
  }

  WorldTileData tile;
  tile.tileTypeId = tiles[tileIndex];
  tile.decorationTypeId = decorations[tileIndex];
  tile.resourceTypeId = resources[tileIndex];

  if (tile.resourceTypeId != 0) {
    tile.resourceVolume = resourceVolumesPacked[resourceVolumeIndex++];
  }

  if (tile.decorationTypeId != 0) {
    tile.decorationState = decorationStatesPacked[decorationStateIndex++];
  }

  ++tileIndex;
  return tile;
}

void JsonFileStorage::LoadFromFile() {
  if (initialized) return;

  std::ifstream file(path);
  if (!file) {
    throw GameError("Failed to open world save file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  LoadFromJson(buffer.str());
  ValidateLoadedData();
  initialized = true;
}

void JsonFileStorage::LoadFromJson(const std::string& json_text) {
  json j;
  try {
    j = json::parse(json_text);
  } catch (const std::exception& ex) {
    throw GameError(std::string("Failed to parse world save JSON: ") + ex.what());
  }

  const int version = JsonRequire::Field<int>(j, "saveVersion", ThrowGameError);
  if (version != 1) {
    throw GameError("Unsupported saveVersion: " + std::to_string(version));
  }

  const json& world = JsonRequire::Object(j, "world", ThrowGameError);
  width = JsonRequire::Field<int>(world, "width", ThrowGameError);
  height = JsonRequire::Field<int>(world, "height", ThrowGameError);
  ValidateEncoding(world);

  meta = {};
  meta.width = width;
  meta.height = height;
  meta.tileTypeNamesById = BuildNamesById(world, "tileTypes");
  meta.decorationNamesById = BuildNamesById(world, "decorationTypes");
  meta.resourceNamesById = BuildNamesById(world, "resourceTypes");

  if (j.contains("camera") && j.at("camera").is_object()) {
    const json& cam = j.at("camera");
    const json& offset = JsonRequire::Object(cam, "offset", ThrowGameError);
    const json& target = JsonRequire::Object(cam, "target", ThrowGameError);
    CameraState state;
    state.offset = { JsonRequire::Field<float>(offset, "x", ThrowGameError), JsonRequire::Field<float>(offset, "y", ThrowGameError) };
    state.target = { JsonRequire::Field<float>(target, "x", ThrowGameError), JsonRequire::Field<float>(target, "y", ThrowGameError) };
    state.rotation = JsonRequire::Field<float>(cam, "rotation", ThrowGameError);
    state.zoom = JsonRequire::Field<float>(cam, "zoom", ThrowGameError);
    meta.camera = state;
  }

  const size_t tileCount = static_cast<size_t>(width) * static_cast<size_t>(height);
  tiles = DecodeU16Array(JsonRequire::Field<std::string>(world, "tiles", ThrowGameError), tileCount, "tiles");
  decorations = DecodeU16Array(JsonRequire::Field<std::string>(world, "decorations", ThrowGameError), tileCount, "decorations");
  resources = DecodeU16Array(JsonRequire::Field<std::string>(world, "resources", ThrowGameError), tileCount, "resources");
  resourceVolumesPacked = DecodeU32Array(JsonRequire::Field<std::string>(world, "resourceVolumes", ThrowGameError), "resourceVolumes");
  decorationStatesPacked = DecodeU32Array(JsonRequire::Field<std::string>(world, "decorationStates", ThrowGameError), "decorationStates");
}

void JsonFileStorage::ValidateLoadedData() const {
  if (width <= 0 || height <= 0) {
    throw GameError("World dimensions must be positive");
  }

  const size_t tileCount = static_cast<size_t>(width) * static_cast<size_t>(height);
  if (tiles.size() != tileCount) {
    throw GameError("tiles count mismatch: expected " + std::to_string(tileCount)
      + ", got " + std::to_string(tiles.size()));
  }
  if (decorations.size() != tileCount) {
    throw GameError("decorations count mismatch: expected " + std::to_string(tileCount)
      + ", got " + std::to_string(decorations.size()));
  }
  if (resources.size() != tileCount) {
    throw GameError("resources count mismatch: expected " + std::to_string(tileCount)
      + ", got " + std::to_string(resources.size()));
  }

  size_t nonZeroResources = 0;
  size_t nonZeroDecorations = 0;
  for (size_t i = 0; i < tileCount; ++i) {
    if (tiles[i] >= meta.tileTypeNamesById.size() || meta.tileTypeNamesById[tiles[i]].empty()) {
      throw GameError("Unknown tileTypeId=" + std::to_string(tiles[i]) + " at tile index " + std::to_string(i));
    }
    if (decorations[i] != 0) {
      if (decorations[i] >= meta.decorationNamesById.size()
        || meta.decorationNamesById[decorations[i]].empty()) {
        throw GameError("Unknown decorationTypeId=" + std::to_string(decorations[i]) + " at tile index " + std::to_string(i));
      }
      ++nonZeroDecorations;
    }
    if (resources[i] != 0) {
      if (resources[i] >= meta.resourceNamesById.size()
        || meta.resourceNamesById[resources[i]].empty()) {
        throw GameError("Unknown resourceTypeId=" + std::to_string(resources[i]) + " at tile index " + std::to_string(i));
      }
      ++nonZeroResources;
    }
  }

  if (resourceVolumesPacked.size() != nonZeroResources) {
    throw GameError("resourceVolumes count mismatch: expected " + std::to_string(nonZeroResources)
      + ", got " + std::to_string(resourceVolumesPacked.size()));
  }
  if (decorationStatesPacked.size() != nonZeroDecorations) {
    throw GameError("decorationStates count mismatch: expected " + std::to_string(nonZeroDecorations)
      + ", got " + std::to_string(decorationStatesPacked.size()));
  }
}
