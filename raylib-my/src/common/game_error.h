#pragma once

#include <stdexcept>
#include <string>

class GameError : public std::runtime_error {
public:
  explicit GameError(std::string);
  std::string Message() const;
};
