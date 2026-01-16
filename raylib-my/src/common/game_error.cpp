#include "game_error.h"

GameError::GameError(std::string msg):
    message { msg }
{}

std::string GameError::Message() const {
    return message;
}
