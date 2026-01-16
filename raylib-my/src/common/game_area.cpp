#ifndef COMMON_GAME_AREA
#define COMMON_GAME_AREA

#include "rectangle_2d.cpp"
#include "game_object.cpp"
#include "../graphics.cpp"

class GameArea: public GameObject {
  public:
    GameArea(GameObject&, Rectangle2D, int);
    GameArea(const GameArea&) = delete;
    GameArea& operator=(const GameArea&) = delete;
    virtual void HandleInput() override;
    virtual void Update() override;
    virtual void Render(Graphics&) override;
    ~GameArea();
    Rectangle2D position;
    int priority;
  private:
    GameObject& object;
};

GameArea::GameArea(GameObject& obj, Rectangle2D pos, int prt):
  GameObject(),
  position { pos },
  object { obj },
  priority { prt }
{};

void GameArea::HandleInput() {
  object.HandleInput();
}

void GameArea::Update() {
  object.Update();
}

void GameArea::Render(Graphics& grph) {
  object.Render(grph);
}

GameArea::~GameArea() {}

#endif // COMMON_GAME_AREA
