#ifndef WORLD_TERRAIN_TYPE
#define WORLD_TERRAIN_TYPE

#include <iostream>
#include <string>
#include "../common/game_error.cpp"
#include "../graphics.cpp"
#include "../common/position_2d.cpp"

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

WorldTileTerrainType::WorldTileTerrainType(std::string nm, float spd, bool wtr, std::string txr, Rectangle srcRect):
  name { nm },
  baseMovingSpeed { spd },
  texurePath { txr },
  isWater { wtr },
  initialized { false },
  textureSrcRect { srcRect }
{}

bool WorldTileTerrainType::emptyTextureSrcRect() {
  return (textureSrcRect.width == 0) || (textureSrcRect.height == 0);
}

void WorldTileTerrainType::LoadTexture(Graphics& grph) {
  Image fullImage = grph.LoadImage(texurePath.c_str());
  if (!emptyTextureSrcRect()) ImageCrop(&fullImage, textureSrcRect);
  textureObj = grph.LoadTextureFromImage(fullImage);
  textureImage = grph.LoadImageFromTexture(textureObj);
  UnloadImage(fullImage);
  initialized = true;
}

const Texture2D& WorldTileTerrainType::Texture() const {
  if (!initialized) {
    throw GameError("Texture for terran tile " + name + " is used but not loaded");
  }
  return textureObj;
}

const Image& WorldTileTerrainType::TextureImage() const {
  if (!initialized) {
    throw GameError("Texture for terran tile " + name + " is used but not loaded");
  }
  return textureImage;
}

WorldTileTerrainType::~WorldTileTerrainType() {}

#endif // WORLD_TERRAIN_TYPE
