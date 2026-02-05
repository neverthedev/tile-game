#include "game_error.h"

#include <utility>

GameError::GameError(std::string msg):
  std::runtime_error(std::move(msg))
{}

std::string GameError::Message() const {
  return std::string(what());
}
