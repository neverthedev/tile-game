#include "world_save_service.h"

#include <limits>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include "../common/game_error.h"

namespace {
  uint16_t RequireId(const std::unordered_map<std::string, uint16_t>& idsByName,
    const std::string& name,
    const std::string& label) {
    auto it = idsByName.find(name);
    if (it == idsByName.end()) {
      throw GameError("Unknown " + label + " name: " + name);
    }
    return it->second;
  }

  std::vector<std::string> BuildNamesById(const std::set<std::string>& names,
    uint16_t startId,
    std::unordered_map<std::string, uint16_t>& idsByName) {
    const size_t total = names.size() + static_cast<size_t>(startId);
    std::vector<std::string> namesById(total, "");
    uint16_t id = startId;
    for (const std::string& name : names) {
      namesById[id] = name;
      idsByName.emplace(name, id);
      ++id;
    }
    return namesById;
  }
}

WorldSaveService::WorldSaveService(const TilesManager& tiles, WorldDataWriter& dataWriter):
  tilesManager { tiles },
  writer { dataWriter }
{}

void WorldSaveService::SaveWorld(const GameWorld& world) {
  if (world.MapWidth <= 0 || world.MapHeight <= 0) {
    throw GameError("World dimensions must be positive");
  }

  const size_t maxSize = std::numeric_limits<size_t>::max();
  const size_t widthSize = static_cast<size_t>(world.MapWidth);
  const size_t heightSize = static_cast<size_t>(world.MapHeight);
  if (widthSize > maxSize / heightSize) {
    throw GameError("World dimensions are too large to serialize");
  }

  const size_t tileCount = widthSize * heightSize;
  const std::vector<std::string> availableTileNames = tilesManager.TileTypeNames();
  std::unordered_set<std::string> knownTileNames(availableTileNames.begin(), availableTileNames.end());

  std::set<std::string> tileNames;
  std::set<std::string> decorationNames;
  std::set<std::string> resourceNames;

  for (int y = 0; y < world.MapHeight; ++y) {
    for (int x = 0; x < world.MapWidth; ++x) {
      const size_t index = static_cast<size_t>(y) * widthSize + static_cast<size_t>(x);
      if (index >= tileCount) {
        throw GameError("Tile index overflow while scanning world");
      }
      const WorldTile& tile = world.GetTile(static_cast<int>(index));
      const std::string& tileName = tile.TerrainTypeName();
      if (tileName.empty()) {
        throw GameError("Tile type name is empty at x: " + std::to_string(x) + ", y: " + std::to_string(y));
      }
      if (knownTileNames.find(tileName) == knownTileNames.end()) {
        throw GameError("Unknown tile type name: " + tileName);
      }
      tileNames.insert(tileName);

      const WorldTileDecoration* decoration = tile.Decoration();
      if (decoration != nullptr) {
        const std::string& decorationName = decoration->Name();
        if (decorationName.empty()) {
          throw GameError("Decoration name is empty at x: " + std::to_string(x) + ", y: " + std::to_string(y));
        }
        tilesManager.DecorationTypeByName(decorationName);
        decorationNames.insert(decorationName);
      }

      const WorldTileResource* resource = tile.Resource();
      if (resource != nullptr) {
        const std::string& resourceName = resource->Name();
        if (resourceName.empty()) {
          throw GameError("Resource name is empty at x: " + std::to_string(x) + ", y: " + std::to_string(y));
        }
        tilesManager.ResourceTypeByName(resourceName);
        resourceNames.insert(resourceName);
      }
    }
  }

  const size_t maxU16 = static_cast<size_t>(std::numeric_limits<uint16_t>::max());
  if (tileNames.size() > maxU16 + 1) {
    throw GameError("Too many tile types to fit into u16 ids");
  }
  if (decorationNames.size() > maxU16) {
    throw GameError("Too many decoration types to fit into u16 ids");
  }
  if (resourceNames.size() > maxU16) {
    throw GameError("Too many resource types to fit into u16 ids");
  }

  std::unordered_map<std::string, uint16_t> tileIdsByName;
  std::unordered_map<std::string, uint16_t> decorationIdsByName;
  std::unordered_map<std::string, uint16_t> resourceIdsByName;

  WorldMeta meta;
  meta.width = world.MapWidth;
  meta.height = world.MapHeight;
  meta.tileTypeNamesById = BuildNamesById(tileNames, 0, tileIdsByName);
  meta.decorationNamesById = BuildNamesById(decorationNames, 1, decorationIdsByName);
  meta.resourceNamesById = BuildNamesById(resourceNames, 1, resourceIdsByName);

  const GameCamera& camera = world.GetCamera();
  CameraState cameraState;
  cameraState.offset = camera.offset;
  cameraState.target = camera.target;
  cameraState.rotation = camera.rotation;
  cameraState.zoom = camera.zoom;
  meta.camera = cameraState;

  writer.WriteMeta(meta);
  writer.BeginTileWrite();

  for (int y = 0; y < world.MapHeight; ++y) {
    for (int x = 0; x < world.MapWidth; ++x) {
      const size_t index = static_cast<size_t>(y) * widthSize + static_cast<size_t>(x);
      const WorldTile& tile = world.GetTile(static_cast<int>(index));
      WorldTileData data;
      data.tileTypeId = RequireId(tileIdsByName, tile.TerrainTypeName(), "tile type");

      const WorldTileDecoration* decoration = tile.Decoration();
      if (decoration != nullptr) {
        data.decorationTypeId = RequireId(decorationIdsByName, decoration->Name(), "decoration type");
        data.decorationState = decoration->State();
      }

      const WorldTileResource* resource = tile.Resource();
      if (resource != nullptr) {
        data.resourceTypeId = RequireId(resourceIdsByName, resource->Name(), "resource type");
        data.resourceVolume = resource->Volume();
      }

      writer.WriteTile(data);
    }
  }

  writer.EndTileWrite();
}
