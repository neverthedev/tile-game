#include <cmath>
#include <iostream>

#include "common/game_error.h"
#include "menus/factory.h"
#include "graphics/raylib_graphics.h"
#include "game_interface.h"
#include "services/service_locator.h"

const float TILE_W = 64.0f;
const float TILE_H = 32.0f;

const int SCREEN_W = 1024;
const int SCREEN_H = 768;

const int FRAME_RATE = 60;

int main(void) {
  ServiceLocator::Initialize();
  RaylibGraphics graphics { SCREEN_W, SCREEN_H, TILE_W, TILE_H, "IsoGame Test", FRAME_RATE };
  GameInterface interface { SCREEN_W, SCREEN_H };

  graphics.InitScreen();

  ServiceLocator::LoadResources(graphics);

  while (!graphics.Done()) {
    interface.HandleInput(graphics, graphics);
    interface.Update(graphics);
    graphics.BeginDrawing();
      interface.Render(graphics);
    graphics.EndDrawing();
  }

  ServiceLocator::Shutdown();

  return 0;
}
