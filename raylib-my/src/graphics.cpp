#include "graphics.h"

Graphics::Graphics(int s_w, int s_h, float t_w, float t_h, const char *t, int fr):
    ScreenWidth { s_w },
    ScreenHeight { s_h },
    TileWidth { t_w },
    TileHeight { t_h },
    TargetFrameRate { fr },
    Title { t },
    Initialized { false },
    Correction { Position2D {.0f, 0.0f} },
    camera { { ScreenWidth / 2.0f, ScreenHeight / 4.0f }, { 0, 0 }, 0, 1.0f }
{}

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

void Graphics::BeginMode2D(Camera2D camera) {
    ::BeginMode2D(camera);
}

void Graphics::EndMode2D() {
    ::EndMode2D();
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

void Graphics::DrawRectangle(Rectangle2D rect) {
    return ::DrawRectangle(rect.x, rect.y, rect.width, rect.height, BLACK);
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

    Vector2 world = GetScreenToWorld2D(mousePos, camera);
    return ScreenToWorld2D(world);
}

void Graphics::DrawDiamondFrame(Position2D center, Color color, bool dst = false, float thickness = 1.0f) {
    Vector2 top { center.x, center.y - TileHeight * 0.5f };
    Vector2 bottom { center.x, center.y + TileHeight * 0.5f };
    Vector2 left { center.x - TileWidth * 0.5f, center.y };
    Vector2 right { center.x + TileWidth * 0.5f, center.y };

    if (dst && Dst) {
        // Draw to image
        ImageDrawLineEx(Dst, top, right, (int)thickness, color);
        ImageDrawLineEx(Dst, right, bottom, (int)thickness, color);
        ImageDrawLineEx(Dst, bottom, left, (int)thickness, color);
        ImageDrawLineEx(Dst, left, top, (int)thickness, color);
    } else {
        if (thickness <= 1.0f) {
            DrawLineV(top, right, color);
            DrawLineV(right, bottom, color);
            DrawLineV(bottom, left, color);
            DrawLineV(left, top, color);
        } else {
            DrawLineEx(top, right, thickness, color);
            DrawLineEx(right, bottom, thickness, color);
            DrawLineEx(bottom, left, thickness, color);
            DrawLineEx(left, top, thickness, color);
        }
    }
}

bool Graphics::Done() {
    return WindowShouldClose();
}

Graphics::~Graphics() {
    CloseWindow();
}
