#include "graphics.h"
#include "raylib.h"

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

Graphics::Graphics(int s_w, int s_h, float t_w, float t_h, const char *t, int fr):
    ScreenWidth { s_w },
    ScreenHeight { s_h },
    TileWidth { t_w },
    TileHeight { t_h },
    TargetFrameRate { fr },
    Title { t },
    Initialized { false },
    Correction { Position2D {.0f, 0.0f} },
    camera { { ScreenWidth / 2.0f, ScreenHeight / 4.0f }, { 0.0f, 0.0f }, 0.0f, 1.0f }
{}

Camera2D Graphics::ConvertToRaylibCamera(const GrphCamera& grphCamera) const {
    return Camera2D {
        ToRaylibVector2(grphCamera.offset),
        ToRaylibVector2(grphCamera.target),
        grphCamera.rotation,
        grphCamera.zoom
    };
}

void Graphics::InitScreen() {
    InitWindow(ScreenWidth, ScreenHeight, Title);
    SetTargetFPS(TargetFrameRate);
    Initialized = true;
}

void Graphics::BeginDrawing() {
    ::BeginDrawing();
}

void Graphics::EndDrawing() {
    ::EndDrawing();
}

void Graphics::BeginMode2D() {
    Camera2D raylibCam = ConvertToRaylibCamera(camera);
    ::BeginMode2D(raylibCam);
}

void Graphics::EndMode2D() {
    ::EndMode2D();
}

void Graphics::UpdateGrphCamera(const GrphCamera& grphCamera) {
    camera = grphCamera;
}

const GrphCamera& Graphics::GetGrphCamera() {
    return camera;
}

Texture2D Graphics::LoadTextures(const char* filename) {
    return ::LoadTexture(filename);
}

Image Graphics::LoadImage(const char* filename) {
    return ::LoadImage(filename);
}

Image Graphics::LoadImageFromTexture(Texture2D texture) {
    return ::LoadImageFromTexture(texture);
}

Texture2D Graphics::LoadTextureFromImage(Image image) {
    return ::LoadTextureFromImage(image);
}

void Graphics::DrawRectangle(Rectangle2D rect, Color2D color) {
    Color raylibColor = ToRaylibColor(color);
    return ::DrawRectangle(rect.x, rect.y, rect.width, rect.height, raylibColor);
}

Position2D Graphics::GridToScreen(Position2D pos) {
    float sx = (pos.x - pos.y) * TileWidth * 0.5f;
    float sy = (pos.x + pos.y) * TileHeight * 0.5f;

    return { sx, sy };
}

Position2D Graphics::ScreenToWorld2D(Position2D world) {
    float a = world.x / (TileWidth * 0.5f);
    float b = world.y / (TileHeight * 0.5f);

    float gx = (a + b) * 0.5f;
    float gy = (b - a) * 0.5f;

    return { floor(gx), floor(gy) }; // fractional; need to floor/round it
}

Position2D Graphics::MouseToWorld2D() {
    Vector2 mousePos = GetMousePosition();
    Camera2D raylibCam = ConvertToRaylibCamera(camera);
    Vector2 world = GetScreenToWorld2D(mousePos, raylibCam);
    return ScreenToWorld2D(FromRaylibVector2(world));
}

void Graphics::DrawDiamondFrame(Position2D center, Color2D color, bool dst = false, float thickness = 1.0f) {
    Vector2 top { center.x, center.y - TileHeight * 0.5f };
    Vector2 bottom { center.x, center.y + TileHeight * 0.5f };
    Vector2 left { center.x - TileWidth * 0.5f, center.y };
    Vector2 right { center.x + TileWidth * 0.5f, center.y };

    Color raylibColor = ToRaylibColor(color);

    if (dst && Dst) {
        // Draw to image
        ImageDrawLineEx(Dst, top, right, (int)thickness, raylibColor);
        ImageDrawLineEx(Dst, right, bottom, (int)thickness, raylibColor);
        ImageDrawLineEx(Dst, bottom, left, (int)thickness, raylibColor);
        ImageDrawLineEx(Dst, left, top, (int)thickness, raylibColor);
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

bool Graphics::Done() {
    return WindowShouldClose();
}

Graphics::~Graphics() {
    CloseWindow();
}
