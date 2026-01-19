#pragma once

#include <string>

#include "raylib.h"  // Must be before graphics.h to avoid forward declaration conflicts
#include "../common/game_error.h"
#include "../common/position_2d.h"

// Forward declaration
class WorldTile;
class Graphics;

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
