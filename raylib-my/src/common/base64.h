#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Base64 {
  std::vector<uint8_t> Decode(const std::string& input);
}
