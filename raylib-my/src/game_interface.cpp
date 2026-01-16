#include "game_interface.h"

GameInterface::GameInterface(int w, int h):
  screenWidth { w },
  screenHeight { h },
  tilesManager { TilesManager() },
  gameWorld { GameWorld::NewWorld(60, 80, tilesManager) }
{
  currentMenu = &MenuFactory::CreateDecorationMenu({
    float(screenWidth) - 160, 0, 160.0, float(screenHeight)
  }, tilesManager);

  AddArea(gameWorld, { 0, 0, float(screenWidth), float(screenHeight) }, 0);
  AddArea(*currentMenu, currentMenu->Position, 1);
};

void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  // Need to find out how works move - copy constructors in relation to vectors
  // And use it here to avoid calling new explicitly
  // Also need to find out how it would work in case of deletion from vektor.
  // How will it clean memory in this case.
  GameArea* area = new GameArea(obj, pos, priority);
  gameAreas.push_back(area);
  std::sort(gameAreas.begin(), gameAreas.end(), [](const GameArea* a1, const GameArea* a2) {
    return a1->priority < a2->priority;
  });
}

// Must be called after window initialization but
// Before first render
void GameInterface::LoadTextures(Graphics& grph) {
  tilesManager.LoadTextures(grph);
}

void GameInterface::HandleInput() {
  Vector2 mouse = GetMousePosition();

  for(int i = gameAreas.size() - 1; i >= 0; --i) {
    GameArea* gameArea = gameAreas[i];

    Rectangle rect = { gameArea->position.x, gameArea->position.y,
                       gameArea->position.width, gameArea->position.height };

    if (CheckCollisionPointRec(mouse, rect)) {
      gameArea->HandleInput();
      break;
    }
  }
}

void GameInterface::Update() {
  for(GameArea* gameArea: gameAreas) {
    gameArea->Update();
  }
}

void GameInterface::Render(Graphics& grph) {
  for(GameArea* gameArea: gameAreas) {
    gameArea->Render(grph);
  }
}

GameInterface::~GameInterface() {
  for(GameArea* area : gameAreas) {
    delete area;
  }
}
