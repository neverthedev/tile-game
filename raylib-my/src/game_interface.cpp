#include "game_interface.h"

#include "services/service_locator.h"
#include "common/position_2d.h"
#include "common/rectangle_2d.h"
#include "config/game_config.h"
#include "graphics/collision_system.h"
#include "graphics/input_system.h"
#include "graphics/render_system.h"

// TODO: Doesn't follow component pattern, consider to refactor
GameInterface::GameInterface(int w, int h):
  screenWidth { w },
  screenHeight { h },
  gameWorld { GameWorld::NewWorld(
    ServiceLocator::GetConfig().WorldWidth,
    ServiceLocator::GetConfig().WorldHeight
  ) }
{
  currentMenu = MenuFactory::CreateDecorationMenu({
    float(screenWidth) - 160, 0, 160.0, float(screenHeight)
  });

  AddArea(gameWorld, { 0, 0, float(screenWidth), float(screenHeight) }, 0);
  AddArea(*currentMenu, currentMenu->Position, 1);
};

void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  gameAreas.emplace_back(obj, pos, priority);
  sortedIndices.push_back(gameAreas.size() - 1);

  // Sort indices by priority, not the objects themselves
  std::sort(sortedIndices.begin(), sortedIndices.end(),
    [this](size_t i1, size_t i2) {
      return gameAreas[i1].priority < gameAreas[i2].priority;
    });
}

void GameInterface::HandleInput(InputSystem& input, CollisionSystem& collision) {
  Position2D mouse = input.GetMousePosition();

  // Iterate in reverse priority order (highest priority first)
  for(int i = sortedIndices.size() - 1; i >= 0; --i) {
    GameArea& gameArea = gameAreas[sortedIndices[i]];

    if (collision.CheckCollisionPointRec(mouse, gameArea.position)) {
      gameArea.HandleInput(input, collision);
      break;
    }
  }
}

void GameInterface::Update(CollisionSystem& collision) {
  for(size_t idx : sortedIndices) {
    gameAreas[idx].Update(collision);
  }
}

void GameInterface::Render(RenderSystem& renderer) {
  for(size_t idx : sortedIndices) {
    gameAreas[idx].Render(renderer);
  }
}

GameInterface::~GameInterface() = default;
