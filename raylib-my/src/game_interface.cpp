#include "game_interface.h"
#include "services/service_locator.h"
#include "common/position_2d.h"
#include "common/rectangle_2d.h"

// Helper function for collision detection (will be moved to Graphics later)
#include "raylib.h"
static bool CheckCollision(Position2D point, Rectangle2D rect) {
    return ::CheckCollisionPointRec(
        Vector2{point.x, point.y},
        Rectangle{rect.x, rect.y, rect.width, rect.height}
    );
}
static Position2D GetMousePos() {
    Vector2 pos = ::GetMousePosition();
    return Position2D{pos.x, pos.y};
}

// TODO: Doesnt' follow component pattern, consider to refactor
GameInterface::GameInterface(int w, int h):
  screenWidth { w },
  screenHeight { h },
  gameWorld { GameWorld::NewWorld(60, 80) }
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

void GameInterface::HandleInput() {
  Position2D mouse = GetMousePos();

  // Iterate in reverse priority order (highest priority first)
  for(int i = sortedIndices.size() - 1; i >= 0; --i) {
    GameArea& gameArea = gameAreas[sortedIndices[i]];

    if (CheckCollision(mouse, gameArea.position)) {
      gameArea.HandleInput();
      break;
    }
  }
}

void GameInterface::Update() {
  for(size_t idx : sortedIndices) {
    gameAreas[idx].Update();
  }
}

void GameInterface::Render(Graphics& grph) {
  for(size_t idx : sortedIndices) {
    gameAreas[idx].Render(grph);
  }
}

GameInterface::~GameInterface() = default;
