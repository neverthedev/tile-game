#pragma once

#include <cstdint>

class TextureHandle {
public:
  TextureHandle();
  explicit TextureHandle(uint32_t id);

  uint32_t GetId() const;
  bool IsValid() const;

  bool operator==(const TextureHandle& other) const;
  bool operator!=(const TextureHandle& other) const;

private:
  uint32_t id;
};
