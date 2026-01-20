#pragma once

#include "../common/color_2d.h"
#include "../common/image_handle.h"
#include "../common/position_2d.h"
#include "../common/rectangle_2d.h"
#include "../common/texture_handle.h"

class ResourcesSystem {
public:
  virtual ~ResourcesSystem() = default;

  virtual TextureHandle LoadTexture(const char* filename) = 0;
  virtual ImageHandle LoadImage(const char* filename) = 0;
  virtual ImageHandle LoadImageFromTexture(TextureHandle texture) = 0;
  virtual TextureHandle LoadTextureFromImage(ImageHandle image) = 0;
  virtual void UnloadTexture(TextureHandle texture) = 0;
  virtual void UnloadImage(ImageHandle image) = 0;

  // Image manipulation operations
  virtual void ImageCrop(ImageHandle& image, Rectangle2D rect) = 0;
  virtual void ImageDraw(ImageHandle dst, ImageHandle src, Rectangle2D srcRect, Rectangle2D dstRect, Color2D tint) = 0;
  virtual void ImageDrawLineEx(ImageHandle dst, Position2D start, Position2D end, float thickness, Color2D color) = 0;

  virtual ImageHandle GenImageColor(float width, float height, Color2D color) = 0;

  // Query methods
  virtual int GetImageWidth(ImageHandle image) const = 0;
  virtual int GetImageHeight(ImageHandle image) const = 0;
};
