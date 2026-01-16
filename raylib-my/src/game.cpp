#include <cmath>
#include <iostream>

#include "common/game_error.h"
#include "menus/factory.h"
#include "graphics.h"
#include "game_interface.h"

const float TILE_W = 64.0f;
const float TILE_H = 32.0f;

const int SCREEN_W = 1024;
const int SCREEN_H = 768;

const int FRAME_RATE = 60;

int main(void) {
  Graphics graphics { SCREEN_W, SCREEN_H, TILE_W, TILE_H, "IsoGame Test", FRAME_RATE };
  GameInterface interface { SCREEN_W, SCREEN_H };

  graphics.InitScreen();

  interface.LoadTextures(graphics);

  while (!graphics.Done()) {
    interface.HandleInput();
    interface.Update();
    graphics.BeginDrawing();
      interface.Render(graphics);
    graphics.EndDrawing();
  }

  return 0;
}
