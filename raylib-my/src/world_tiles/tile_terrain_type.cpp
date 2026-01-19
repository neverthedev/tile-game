#include "tile_terrain_type.h"
#include "../graphics.h"

#include <iostream>

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
