#include "raylib_graphics.h"

#include <unordered_map>

#include "raylib.h"
#include "../common/game_error.h"

// PImpl implementation to keep Raylib types out of the public header
struct RaylibGraphics::Impl {
  std::unordered_map<uint32_t, Texture2D> textures;
  std::unordered_map<uint32_t, Image> images;
  uint32_t nextTextureId = 1;
  uint32_t nextImageId = 1;
  Camera2D lastCamera;
};

// Helper functions for converting between our types and raylib types
static Vector2 ToRaylibVector2(const Position2D& pos) {
  return Vector2 { pos.x, pos.y };
}

static Position2D FromRaylibVector2(const Vector2& vec) {
  return Position2D { vec.x, vec.y };
}

static Color ToRaylibColor(const Color2D& color) {
  return Color { color.r, color.g, color.b, color.a };
}

static Rectangle ToRaylibRectangle(const Rectangle2D& rect) {
  return Rectangle { rect.x, rect.y, rect.width, rect.height };
}

RaylibGraphics::RaylibGraphics(int s_w, int s_h, float t_w, float t_h, const char *t, int fr):
  ScreenWidth { s_w },
  ScreenHeight { s_h },
  TileWidth { t_w },
  TileHeight { t_h },
  TargetFrameRate { fr },
  Title { t },
  Initialized { false },
  Correction { Position2D {.0f, 0.0f} },
  camera { { ScreenWidth / 2.0f, ScreenHeight / 4.0f }, { 0.0f, 0.0f }, 0.0f, 1.0f },
  impl { std::make_unique<Impl>() }
{}


void* RaylibGraphics::ConvertToRaylibCamera() {
  // store Camera2D in impl and return pointer to it
  impl->lastCamera = Camera2D {
    ToRaylibVector2(camera.offset),
    ToRaylibVector2(camera.target),
    camera.rotation,
    camera.zoom
  };
  return static_cast<void*>(&impl->lastCamera);
}

void RaylibGraphics::InitScreen() {
  InitWindow(ScreenWidth, ScreenHeight, Title);
  SetTargetFPS(TargetFrameRate);
  Initialized = true;
}

void RaylibGraphics::BeginDrawing() {
  ::BeginDrawing();
}

void RaylibGraphics::EndDrawing() {
  ::EndDrawing();
}

void RaylibGraphics::BeginMode2D() {
  Camera2D* raylibCam = static_cast<Camera2D*>(ConvertToRaylibCamera());
  ::BeginMode2D(*raylibCam);
}

void RaylibGraphics::EndMode2D() {
  ::EndMode2D();
}

void RaylibGraphics::UpdateGrphCamera(const GrphCamera& grphCamera) {
  camera = grphCamera;
}

const GrphCamera& RaylibGraphics::GetGrphCamera() const {
  return camera;
}

float RaylibGraphics::GetTileWidth() const {
  return TileWidth;
}

float RaylibGraphics::GetTileHeight() const {
  return TileHeight;
}

Position2D RaylibGraphics::GetCorrection() const {
  return Correction;
}

ImageHandle RaylibGraphics::GetDst() const {
  return Dst;
}

void RaylibGraphics::ClearBackground(Color2D color) {
  Color raylibColor = ToRaylibColor(color);
  ::ClearBackground(raylibColor);
}

void RaylibGraphics::DrawTexture(TextureHandle textureHandle, Position2D position, Color2D tint) {
  if (!textureHandle.IsValid()) return;
    const Texture2D& tex = impl->textures.at(textureHandle.GetId());
  Color raylibTint = ToRaylibColor(tint);
  ::DrawTexture(tex, (int)position.x, (int)position.y, raylibTint);
}

void RaylibGraphics::DrawFPS(int x, int y) {
  ::DrawFPS(x, y);
}

void RaylibGraphics::SetDst(ImageHandle dst) {
  Dst = dst;
}

void RaylibGraphics::SetCorrection(Position2D correction) {
  Correction = correction;
}

TextureHandle RaylibGraphics::LoadTexture(const char* filename) {
  Texture2D tex = ::LoadTexture(filename);
  uint32_t id = impl->nextTextureId++;
  impl->textures[id] = tex;
  return TextureHandle(id);
}

ImageHandle RaylibGraphics::LoadImage(const char* filename) {
  Image img = ::LoadImage(filename);
  uint32_t id = impl->nextImageId++;
  impl->images[id] = img;
  return ImageHandle(id);
}

ImageHandle RaylibGraphics::LoadImageFromTexture(TextureHandle textureHandle) {
  const Texture2D& tex = impl->textures.at(textureHandle.GetId());
  Image img = ::LoadImageFromTexture(tex);
  uint32_t id = impl->nextImageId++;
  impl->images[id] = img;
  return ImageHandle(id);
}

TextureHandle RaylibGraphics::LoadTextureFromImage(ImageHandle imageHandle) {
  const Image& img = impl->images.at(imageHandle.GetId());
  Texture2D tex = ::LoadTextureFromImage(img);
  uint32_t id = impl->nextTextureId++;
  impl->textures[id] = tex;
  return TextureHandle(id);
}

void RaylibGraphics::UnloadTexture(TextureHandle textureHandle) {
  if (!textureHandle.IsValid()) return;
  auto it = impl->textures.find(textureHandle.GetId());
  if (it != impl->textures.end()) {
    ::UnloadTexture(it->second);
    impl->textures.erase(it);
  }
}

void RaylibGraphics::UnloadImage(ImageHandle imageHandle) {
  if (!imageHandle.IsValid()) return;
  auto it = impl->images.find(imageHandle.GetId());
  if (it != impl->images.end()) {
    ::UnloadImage(it->second);
    impl->images.erase(it);
  }
}

void RaylibGraphics::ImageCrop(ImageHandle& imageHandle, Rectangle2D rect) {
  if (!imageHandle.IsValid()) return;
  Image& img = impl->images.at(imageHandle.GetId());
  Rectangle raylibRect = ToRaylibRectangle(rect);
  ::ImageCrop(&img, raylibRect);
}

void RaylibGraphics::ImageDraw(ImageHandle dstHandle, ImageHandle srcHandle, Rectangle2D srcRect, Rectangle2D dstRect, Color2D tint) {
  if (!dstHandle.IsValid() || !srcHandle.IsValid()) return;
  Image& dst = impl->images.at(dstHandle.GetId());
  const Image& src = impl->images.at(srcHandle.GetId());
  Rectangle raylibSrcRect = ToRaylibRectangle(srcRect);
  Rectangle raylibDstRect = ToRaylibRectangle(dstRect);
  Color raylibTint = ToRaylibColor(tint);
  ::ImageDraw(&dst, src, raylibSrcRect, raylibDstRect, raylibTint);
}

void RaylibGraphics::ImageDrawLineEx(ImageHandle dstHandle, Position2D start, Position2D end, float thickness, Color2D color) {
  if (!dstHandle.IsValid()) return;
  Image& dst = impl->images.at(dstHandle.GetId());
  Vector2 raylibStart = ToRaylibVector2(start);
  Vector2 raylibEnd = ToRaylibVector2(end);
  Color raylibColor = ToRaylibColor(color);
  ::ImageDrawLineEx(&dst, raylibStart, raylibEnd, (int)thickness, raylibColor);
}

ImageHandle RaylibGraphics::GenImageColor(float width, float height, Color2D color) {
  Color raylibColor = ToRaylibColor(color);
  Image img = ::GenImageColor((int)width, (int)height, raylibColor);
  uint32_t id = impl->nextImageId++;
  impl->images[id] = img;
  return ImageHandle(id);
}

int RaylibGraphics::GetImageWidth(ImageHandle handle) const {
  if (!handle.IsValid()) return 0;
  const Image& img = impl->images.at(handle.GetId());
  return img.width;
}

int RaylibGraphics::GetImageHeight(ImageHandle handle) const {
  if (!handle.IsValid()) return 0;
  const Image& img = impl->images.at(handle.GetId());
  return img.height;
}


void RaylibGraphics::DrawRectangle(Rectangle2D rect, Color2D color) {
  Color raylibColor = ToRaylibColor(color);
  return ::DrawRectangle(rect.x, rect.y, rect.width, rect.height, raylibColor);
}

Position2D RaylibGraphics::GridToScreen(Position2D pos) {
  float sx = (pos.x - pos.y) * TileWidth * 0.5f;
  float sy = (pos.x + pos.y) * TileHeight * 0.5f;

  return { sx, sy };
}

Position2D RaylibGraphics::ScreenToWorld2D(Position2D world) {
  float a = world.x / (TileWidth * 0.5f);
  float b = world.y / (TileHeight * 0.5f);

  float gx = (a + b) * 0.5f;
  float gy = (b - a) * 0.5f;

  return { floor(gx), floor(gy) }; // fractional; need to floor/round it
}

Position2D RaylibGraphics::MouseToWorld2D() {
  Vector2 mousePos = ::GetMousePosition();
  Camera2D* raylibCam = static_cast<Camera2D*>(ConvertToRaylibCamera());
  Vector2 world = GetScreenToWorld2D(mousePos, *raylibCam);
  return ScreenToWorld2D(FromRaylibVector2(world));
}

void RaylibGraphics::DrawDiamondFrame(Position2D center, Color2D color, bool dst = false, float thickness = 1.0f) {
  Vector2 top { center.x, center.y - TileHeight * 0.5f };
  Vector2 bottom { center.x, center.y + TileHeight * 0.5f };
  Vector2 left { center.x - TileWidth * 0.5f, center.y };
  Vector2 right { center.x + TileWidth * 0.5f, center.y };

  Color raylibColor = ToRaylibColor(color);

  if (dst && Dst.IsValid()) {
    // Draw to image using handle
    Image& dstImage = impl->images.at(Dst.GetId());
    ::ImageDrawLineEx(&dstImage, top, right, (int)thickness, raylibColor);
    ::ImageDrawLineEx(&dstImage, right, bottom, (int)thickness, raylibColor);
    ::ImageDrawLineEx(&dstImage, bottom, left, (int)thickness, raylibColor);
    ::ImageDrawLineEx(&dstImage, left, top, (int)thickness, raylibColor);
  } else {
    if (thickness <= 1.0f) {
      DrawLineV(top, right, raylibColor);
      DrawLineV(right, bottom, raylibColor);
      DrawLineV(bottom, left, raylibColor);
      DrawLineV(left, top, raylibColor);
    } else {
      DrawLineEx(top, right, thickness, raylibColor);
      DrawLineEx(right, bottom, thickness, raylibColor);
      DrawLineEx(bottom, left, thickness, raylibColor);
      DrawLineEx(left, top, thickness, raylibColor);
    }
  }
}

bool RaylibGraphics::Done() {
  return WindowShouldClose();
}

RaylibGraphics::~RaylibGraphics() {
  CloseWindow();
}

// InputSystem implementation
Position2D RaylibGraphics::GetMousePosition() const {
  Vector2 pos = ::GetMousePosition();
  return FromRaylibVector2(pos);
}

bool RaylibGraphics::IsKeyPressed(int key) const {
  return ::IsKeyPressed(key);
}

bool RaylibGraphics::IsMouseButtonPressed(int button) const {
  return ::IsMouseButtonPressed(button);
}

bool RaylibGraphics::IsKeyDown(int key) const {
  return ::IsKeyDown(key);
}

float RaylibGraphics::GetMouseWheelMove() const {
  return ::GetMouseWheelMove();
}

// CollisionSystem implementation
bool RaylibGraphics::CheckCollisionPointRec(Position2D point, Rectangle2D rect) const {
  Vector2 raylibPoint = ToRaylibVector2(point);
  Rectangle raylibRect = ToRaylibRectangle(rect);
  return ::CheckCollisionPointRec(raylibPoint, raylibRect);
}

bool RaylibGraphics::CheckCollisionRecs(Rectangle2D rec1, Rectangle2D rec2) const {
  Rectangle raylibRec1 = ToRaylibRectangle(rec1);
  Rectangle raylibRec2 = ToRaylibRectangle(rec2);
  return ::CheckCollisionRecs(raylibRec1, raylibRec2);
}
