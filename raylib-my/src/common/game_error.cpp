#ifndef COMMON_GAME_ERROR_H
#define COMMON_GAME_ERROR_H

#include <string>

class GameError {
  public:
    GameError(std::string);
    std::string Message() const;
  private:
    std::string message;
};

GameError::GameError(std::string msg):
  message { msg }
{}

std::string GameError::Message() const {
  return message;
}

#endif // COMMON_GAME_ERROR_H
