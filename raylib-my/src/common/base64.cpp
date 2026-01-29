#include "base64.h"

#include "game_error.h"

namespace {
  int AlphabetValue(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '=') return -2;
    return -1;
  }
}

std::vector<uint8_t> Base64::Decode(const std::string& input) {
  std::vector<uint8_t> out;
  out.reserve((input.size() / 4) * 3);

  uint32_t buffer = 0;
  int bits = 0;
  int padding = 0;

  for (char c : input) {
    if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
      continue;
    }

    int val = AlphabetValue(c);
    if (val == -1) {
      throw GameError("Invalid base64 character encountered");
    }
    if (val == -2) {
      ++padding;
      val = 0;
    }

    buffer = (buffer << 6) | static_cast<uint32_t>(val);
    bits += 6;

    if (bits >= 8) {
      bits -= 8;
      uint8_t byte = static_cast<uint8_t>((buffer >> bits) & 0xFFu);
      out.push_back(byte);
    }
  }

  if (padding > 0) {
    if (out.size() < static_cast<size_t>(padding)) {
      throw GameError("Invalid base64 padding");
    }
    out.resize(out.size() - static_cast<size_t>(padding));
  }

  return out;
}
