#include <cmath>
#include <iostream>

#include "common/game_error.h"
#include "menus/factory.h"
#include "graphics/raylib_graphics.h"
#include "graphics/resources_system.h"
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

  ServiceLocator::LoadResources(static_cast<ResourcesSystem&>(graphics));

  while (!graphics.Done()) {
    interface.HandleInput(static_cast<InputSystem&>(graphics), static_cast<CollisionSystem&>(graphics));
    interface.Update(static_cast<CollisionSystem&>(graphics));
    graphics.BeginDrawing();
      interface.Render(static_cast<RenderSystem&>(graphics));
    graphics.EndDrawing();
  }

  ServiceLocator::Shutdown();

  return 0;
}
