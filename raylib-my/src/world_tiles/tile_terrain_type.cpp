#include "tile_terrain_type.h"
#include "../graphics/raylib_graphics.h"

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

void WorldTileTerrainType::LoadTexture(RaylibGraphics& grph) {
  ImageHandle fullImage = grph.LoadImage(texurePath.c_str());
  if (!emptyTextureSrcRect()) {
    Rectangle2D cropRect { textureSrcRect.x, textureSrcRect.y, textureSrcRect.width, textureSrcRect.height };
    grph.ImageCrop(fullImage, cropRect);
  }
  textureObj = grph.LoadTextureFromImage(fullImage);
  textureImage = grph.LoadImageFromTexture(textureObj);
  grph.UnloadImage(fullImage);
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
