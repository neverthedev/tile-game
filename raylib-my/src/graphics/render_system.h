#pragma once

#include "../common/color_2d.h"
#include "../common/grph_camera.h"
#include "../common/image_handle.h"
#include "../common/position_2d.h"
#include "../common/rectangle_2d.h"
#include "../common/texture_handle.h"

class RenderSystem {
public:
  virtual ~RenderSystem() = default;

  virtual void BeginDrawing() = 0;
  virtual void EndDrawing() = 0;
  virtual void BeginMode2D() = 0;
  virtual void EndMode2D() = 0;
  virtual void DrawRectangle(Rectangle2D rect, Color2D color) = 0;
  virtual void DrawDiamondFrame(Position2D center, Color2D color, bool dst, float thickness) = 0;

  // Camera management
  virtual void UpdateGrphCamera(const GrphCamera& camera) = 0;
  virtual const GrphCamera& GetGrphCamera() const = 0;

  // Coordinate transformations
  virtual Position2D GridToScreen(Position2D pos) = 0;
  virtual Position2D ScreenToWorld2D(Position2D screenPos) = 0;
  virtual Position2D ScreenToWorldWithCamera(const GrphCamera& camera) = 0;
  virtual Position2D MouseToWorld2D() = 0;

  // Tile rendering properties (public members or getters)
  virtual float GetTileWidth() const = 0;
  virtual float GetTileHeight() const = 0;
  virtual Position2D GetCorrection() const = 0;
  virtual ImageHandle GetDst() const = 0;

  // Image operations needed for rendering
  virtual void ImageDraw(ImageHandle dst, ImageHandle src, Rectangle2D srcRect, Rectangle2D dstRect, Color2D tint) = 0;
  virtual int GetImageWidth(ImageHandle image) const = 0;
  virtual int GetImageHeight(ImageHandle image) const = 0;

  // Rendering operations
  virtual void ClearBackground(Color2D color) = 0;
  virtual void DrawTexture(TextureHandle texture, Position2D position, Color2D tint) = 0;
  virtual void DrawFPS(int x, int y) = 0;

  // Tile rendering state management
  virtual void SetDst(ImageHandle dst) = 0;
  virtual void SetCorrection(Position2D correction) = 0;

  // Resource operations
  virtual ImageHandle GenImageColor(float width, float height, Color2D color) = 0;
  virtual TextureHandle LoadTextureFromImage(ImageHandle image) = 0;
  virtual void UnloadTexture(TextureHandle texture) = 0;
};
