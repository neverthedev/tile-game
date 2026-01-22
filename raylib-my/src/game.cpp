#include <cmath>
#include <exception>
#include <iostream>

#include "common/game_error.h"
#include "config/game_config.h"
#include "menus/factory.h"
#include "graphics/raylib_graphics.h"
#include "graphics/resources_system.h"
#include "game_interface.h"
#include "services/service_locator.h"

int main(void) {
  GameConfig config;
  try {
    config = GameConfig::LoadFromFile("config/config.json");
  } catch (const std::exception& ex) {
    std::cerr << "Failed to load config: " << ex.what() << std::endl;
    return 1;
  }

  ServiceLocator::Initialize(config);
  RaylibGraphics graphics {
    config.ScreenWidth,
    config.ScreenHeight,
    config.TileWidth,
    config.TileHeight,
    config.WindowTitle,
    config.FrameRate
  };
  GameInterface interface { config.ScreenWidth, config.ScreenHeight };

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
