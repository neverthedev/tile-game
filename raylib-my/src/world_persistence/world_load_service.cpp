#include "world_load_service.h"

#include <limits>
#include <string>

WorldLoadService::WorldLoadService(const TilesManager& tilesMngr, WorldDataReader& rdr):
  reader { rdr },
  worldMeta { },
  tilesManager { tilesMngr }
{}

std::unique_ptr<WorldTileDecoration> WorldLoadService::BuildDecoration(const std::string& decoration_name) const {
  if (decoration_name == "None") return nullptr;

  auto it = decorationTypes.find(decoration_name);
  if (it == decorationTypes.end()) {
    throw GameError("Unknown decoration type name: " + decoration_name);
  }

  return std::make_unique<WorldTileDecoration>(it->second, decoration_name);
}

std::unique_ptr<WorldTileResource> WorldLoadService::BuildResource(const std::string& resource_name, uint32_t volume) const {
  if (resource_name == "None") return nullptr;

  if (volume > static_cast<uint32_t>(std::numeric_limits<int>::max())) {
    throw GameError("Resource volume too large: " + std::to_string(volume));
  }

  const int volumeInt = static_cast<int>(volume);

  auto it = resourceTypes.find(resource_name);
  if (it == resourceTypes.end()) {
    throw GameError("Unknown resource type name: " + resource_name);
  }

  return std::make_unique<WorldTileResource>(it->second, resource_name, volumeInt);
}

std::unique_ptr<WorldTile> WorldLoadService::ProvideTile(int x, int y) const {
  std::optional<WorldTileData> tileDataOpt = reader.NextTile();
  if (!tileDataOpt.has_value()) {
    throw GameError("Unexpected end of tile stream at x: " + std::to_string(x) + ", y: " + std::to_string(y));
  }
  const WorldTileData& tileData = *tileDataOpt;

  if (tileData.tileTypeId >= worldMeta.tileTypeNamesById.size()) {
    throw GameError("Unknown tileTypeId=" + std::to_string(tileData.tileTypeId) +
      " at x: " + std::to_string(x) + ", y: " + std::to_string(y));
  }

  const Position2D pos { static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f };

  const std::string& tileTypeName = worldMeta.tileTypeNamesById[tileData.tileTypeId];
  if (tileTypeName.empty()) {
    throw GameError("Missing tile type name for tileTypeId=" + std::to_string(tileData.tileTypeId));
  }

  std::unique_ptr<WorldTile> tile { tilesManager.NewTile(tileTypeName, pos) };

  if (tileData.decorationTypeId != 0) {
    if (tileData.decorationTypeId >= worldMeta.decorationNamesById.size()) {
      throw GameError("Unknown decorationTypeId=" + std::to_string(tileData.decorationTypeId) +
        " at x: " + std::to_string(x) + ", y: " + std::to_string(y));
    }
    const std::string& decorationName = worldMeta.decorationNamesById[tileData.decorationTypeId];
    tile->Decoration = BuildDecoration(decorationName);
  }

  if (tileData.resourceTypeId != 0) {
    if (tileData.resourceTypeId >= worldMeta.resourceNamesById.size()) {
      throw GameError("Unknown resourceTypeId=" + std::to_string(tileData.resourceTypeId) +
        " at x: " + std::to_string(x) + ", y: " + std::to_string(y));
    }
    if (!tileData.resourceVolume.has_value()) {
      throw GameError("Missing resource volume at x: " + std::to_string(x) + ", y: " + std::to_string(y));
    }
    const std::string& resourceName = worldMeta.resourceNamesById[tileData.resourceTypeId];
    tile->Resource = BuildResource(resourceName, *tileData.resourceVolume);
  }

  return tile;
}

std::unique_ptr<GameWorld> WorldLoadService::BuildWorld() {
  worldMeta = reader.ReadMeta();

  if (worldMeta.width <= 0 || worldMeta.height <= 0) {
    throw GameError("World dimensions must be positive");
  }

  const size_t tileCount = static_cast<size_t>(worldMeta.width) * static_cast<size_t>(worldMeta.height);
  reader.BeginTileScan();

  auto world = GameWorld::NewWorld(worldMeta.width, worldMeta.height, [this](int x, int y) {
    return ProvideTile(x, y);
  });

  if (reader.NextTile().has_value()) {
    throw GameError("Extra tile data after expected tile count: " + std::to_string(tileCount));
  }

  if (worldMeta.camera.has_value()) {
    GameCamera& camera = world->GetCamera();
    camera.offset = worldMeta.camera->offset;
    camera.target = worldMeta.camera->target;
    camera.rotation = worldMeta.camera->rotation;
    camera.zoom = worldMeta.camera->zoom;
  }

  return world;
}
