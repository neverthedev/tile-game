#pragma once

#include <string>

#include "../common/game_error.h"
#include "../graphics.h"
#include "../common/position_2d.h"

// Forward declaration
class WorldTile;

class WorldTileTerrainType {
  friend class WorldTile;

public:
  WorldTileTerrainType(std::string, float, bool, std::string, Rectangle);
  WorldTile* NewTile(Position2D) const;
  ~WorldTileTerrainType();
  void LoadTexture(Graphics& grph);
  const Texture2D& Texture() const;
  const Image& TextureImage() const;

private:
  std::string name;
  std::string texurePath;
  bool isWater;
  float baseMovingSpeed;
  Rectangle textureSrcRect;
  Texture2D textureObj;
  Image textureImage;
  bool initialized;

  bool emptyTextureSrcRect();
};
