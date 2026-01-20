#include "tile_terrain_type.h"
#include "../graphics/resources_system.h"

#include <iostream>

WorldTileTerrainType::WorldTileTerrainType(std::string nm, float spd, bool wtr, std::string txr, Rectangle2D srcRect):
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

void WorldTileTerrainType::LoadTexture(ResourcesSystem& resources) {
  ImageHandle fullImage = resources.LoadImage(texurePath.c_str());
  if (!emptyTextureSrcRect()) {
    Rectangle2D cropRect { textureSrcRect.x, textureSrcRect.y, textureSrcRect.width, textureSrcRect.height };
    resources.ImageCrop(fullImage, cropRect);
  }
  textureObj = resources.LoadTextureFromImage(fullImage);
  textureImage = resources.LoadImageFromTexture(textureObj);
  resources.UnloadImage(fullImage);
  initialized = true;
}

TextureHandle WorldTileTerrainType::Texture() const {
  if (!initialized) {
    throw GameError("Texture for terran tile " + name + " is used but not loaded");
  }
  return textureObj;
}

ImageHandle WorldTileTerrainType::TextureImage() const {
  if (!initialized) {
    throw GameError("Texture for terran tile " + name + " is used but not loaded");
  }
  return textureImage;
}

WorldTileTerrainType::~WorldTileTerrainType() {}
