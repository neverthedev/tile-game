#pragma once

#include <cmath>

#include "raylib.h"
#include "common/position_2d.h"
#include "common/rectangle_2d.h"
#include "common/color_2d.h"
#include "common/grph_camera.h"

class Graphics {
public:
    int ScreenWidth;
    int ScreenHeight;
    float TileWidth;
    float TileHeight;
    int TargetFrameRate;
    const char *Title;
    bool Initialized;
    Image* Dst;
    Position2D Correction;

    Graphics(int, int, float, float, const char *, int);
    void InitScreen();
    void BeginDrawing();
    void EndDrawing();
    void BeginMode2D();
    void EndMode2D();
    bool Done();
    Texture2D LoadTextures(const char* filename);
    Image LoadImage(const char* filename);
    Image LoadImageFromTexture(Texture2D texture);
    Texture2D LoadTextureFromImage(Image image);
    void DrawDiamondFrame(Position2D, Color2D, bool, float);
    void DrawRectangle(Rectangle2D rect, Color2D color);
    ~Graphics();

    Position2D GridToScreen(Position2D pos);
    Position2D ScreenToWorld2D(Position2D);
    Position2D MouseToWorld2D();
    void UpdateGrphCamera(const GrphCamera& grphCamera);
    const GrphCamera& GetGrphCamera();

private:
    GrphCamera camera;
    Camera2D ConvertToRaylibCamera(const GrphCamera& camera) const;
};
