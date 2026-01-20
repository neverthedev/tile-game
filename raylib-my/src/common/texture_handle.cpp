#include "texture_handle.h"

TextureHandle::TextureHandle(): id(0) {}

TextureHandle::TextureHandle(uint32_t id): id(id) {}

uint32_t TextureHandle::GetId() const {
  return id;
}

bool TextureHandle::IsValid() const {
  return id != 0;
}

bool TextureHandle::operator==(const TextureHandle& other) const {
  return id == other.id;
}

bool TextureHandle::operator!=(const TextureHandle& other) const {
  return id != other.id;
}
