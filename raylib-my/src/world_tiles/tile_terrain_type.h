#pragma once

#include <string>

#include "../common/game_error.h"
#include "../common/position_2d.h"
#include "../common/rectangle_2d.h"
#include "../common/texture_handle.h"
#include "../common/image_handle.h"

// Forward declaration
class WorldTile;
class ResourcesSystem;

class WorldTileTerrainType {
  friend class WorldTile;

public:
  WorldTileTerrainType(std::string, float, bool, std::string, Rectangle2D);
  WorldTile* NewTile(Position2D) const;
  ~WorldTileTerrainType();
  void LoadTexture(ResourcesSystem& resources);
  TextureHandle Texture() const;
  ImageHandle TextureImage() const;
  WorldTileTerrainType(const WorldTileTerrainType&) = delete;
  WorldTileTerrainType(WorldTileTerrainType&&) = delete;
  WorldTileTerrainType& operator=(const WorldTileTerrainType&) = delete;
  WorldTileTerrainType& operator=(WorldTileTerrainType&&) = delete;

private:
  std::string name;
  std::string texurePath;
  bool isWater;
  float baseMovingSpeed;
  Rectangle2D textureSrcRect;
  TextureHandle textureObj;
  ImageHandle textureImage;
  bool initialized;

  bool emptyTextureSrcRect();
};
