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

std::string Base64::Encode(const std::vector<uint8_t>& input) {
  static const char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  std::string out;
  out.reserve(((input.size() + 2) / 3) * 4);

  size_t i = 0;
  while (i + 2 < input.size()) {
    uint32_t trio = (static_cast<uint32_t>(input[i]) << 16)
      | (static_cast<uint32_t>(input[i + 1]) << 8)
      | static_cast<uint32_t>(input[i + 2]);
    out.push_back(alphabet[(trio >> 18) & 0x3F]);
    out.push_back(alphabet[(trio >> 12) & 0x3F]);
    out.push_back(alphabet[(trio >> 6) & 0x3F]);
    out.push_back(alphabet[trio & 0x3F]);
    i += 3;
  }

  const size_t remaining = input.size() - i;
  if (remaining == 1) {
    uint32_t duo = static_cast<uint32_t>(input[i]) << 16;
    out.push_back(alphabet[(duo >> 18) & 0x3F]);
    out.push_back(alphabet[(duo >> 12) & 0x3F]);
    out.push_back('=');
    out.push_back('=');
  } else if (remaining == 2) {
    uint32_t duo = (static_cast<uint32_t>(input[i]) << 16)
      | (static_cast<uint32_t>(input[i + 1]) << 8);
    out.push_back(alphabet[(duo >> 18) & 0x3F]);
    out.push_back(alphabet[(duo >> 12) & 0x3F]);
    out.push_back(alphabet[(duo >> 6) & 0x3F]);
    out.push_back('=');
  }

  return out;
}
