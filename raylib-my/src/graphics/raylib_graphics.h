#pragma once

#include <cmath>
#include <memory>
#include <string>

#include "collision_system.h"
#include "input_system.h"
#include "render_system.h"
#include "resources_system.h"
#include "../common/keyboard_2d.h"
#include "../common/color_2d.h"
#include "../common/grph_camera.h"
#include "../common/image_handle.h"
#include "../common/position_2d.h"
#include "../common/rectangle_2d.h"
#include "../common/texture_handle.h"

class RaylibGraphics : public InputSystem,
                       public CollisionSystem,
                       public RenderSystem,
                       public ResourcesSystem {
public:
  int ScreenWidth;
  int ScreenHeight;
  float TileWidth;
  float TileHeight;
  int TargetFrameRate;
  std::string Title;
  bool Initialized;
  ImageHandle Dst;
  Position2D Correction;

  RaylibGraphics(int, int, float, float, const std::string, int);
  void InitScreen();
  bool Done();
  ~RaylibGraphics();

  // InputSystem interface implementation
  Position2D GetMousePosition() const override;
  bool IsKeyPressed(Keyboard2D::Key key) const override;
  bool IsMouseButtonPressed(int button) const override;
  bool IsKeyDown(Keyboard2D::Key key) const override;
  float GetMouseWheelMove() const override;

  // CollisionSystem interface implementation
  bool CheckCollisionPointRec(Position2D point, Rectangle2D rect) const override;
  bool CheckCollisionRecs(Rectangle2D rec1, Rectangle2D rec2) const override;

  // RenderSystem interface implementation
  void BeginDrawing() override;
  void EndDrawing() override;
  void BeginMode2D() override;
  void EndMode2D() override;
  void DrawRectangle(Rectangle2D rect, Color2D color) override;
  void DrawDiamondFrame(Position2D center, Color2D color, bool dst, float thickness) override;
  void UpdateGrphCamera(const GrphCamera& camera) override;
  const GrphCamera& GetGrphCamera() const override;
  Position2D GridToScreen(Position2D pos) override;
  Position2D ScreenToWorld2D(Position2D screenPos) override;
  Position2D ScreenToWorldWithCamera(const GrphCamera& camera) override;
  Position2D MouseToWorld2D() override;
  float GetTileWidth() const override;
  float GetTileHeight() const override;
  Position2D GetCorrection() const override;
  ImageHandle GetDst() const override;
  void ImageDraw(ImageHandle dst, ImageHandle src, Rectangle2D srcRect, Rectangle2D dstRect, Color2D tint) override;
  int GetImageWidth(ImageHandle image) const override;
  int GetImageHeight(ImageHandle image) const override;
  void ClearBackground(Color2D color) override;
  void DrawTexture(TextureHandle texture, Position2D position, Color2D tint, float scale = 1.0f) override;
  void DrawText(const char* text, Position2D position, int fontSize, Color2D color) override;
  void DrawFPS(int x, int y) override;
  void SetDst(ImageHandle dst) override;
  void SetCorrection(Position2D correction) override;

  // ResourcesSystem interface implementation
  TextureHandle LoadTexture(const char* filename) override;
  ImageHandle LoadImage(const char* filename) override;
  ImageHandle LoadImageFromTexture(TextureHandle texture) override;
  TextureHandle LoadTextureFromImage(ImageHandle image) override;
  void UnloadTexture(TextureHandle texture) override;
  void UnloadImage(ImageHandle image) override;
  void ImageCrop(ImageHandle& image, Rectangle2D rect) override;
  void ImageDrawLineEx(ImageHandle dst, Position2D start, Position2D end, float thickness, Color2D color) override;
  ImageHandle GenImageColor(float width, float height, Color2D color) override;

private:
  GrphCamera camera;
  struct Impl;
  std::unique_ptr<Impl> impl;
  void* ConvertToRaylibCamera();
};
