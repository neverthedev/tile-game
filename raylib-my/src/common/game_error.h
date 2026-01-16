#pragma once

#include <string>

class GameError {
public:
    GameError(std::string);
    std::string Message() const;

private:
    std::string message;
};
