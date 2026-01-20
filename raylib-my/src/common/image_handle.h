#pragma once

#include <cstdint>

class ImageHandle {
public:
  ImageHandle();
  explicit ImageHandle(uint32_t id);

  uint32_t GetId() const;
  bool IsValid() const;

  bool operator==(const ImageHandle& other) const;
  bool operator!=(const ImageHandle& other) const;

private:
  uint32_t id;
};
