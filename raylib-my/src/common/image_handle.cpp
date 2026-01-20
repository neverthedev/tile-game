#include "image_handle.h"

ImageHandle::ImageHandle(): id(0) {}

ImageHandle::ImageHandle(uint32_t id): id(id) {}

uint32_t ImageHandle::GetId() const {
  return id;
}

bool ImageHandle::IsValid() const {
  return id != 0;
}

bool ImageHandle::operator==(const ImageHandle& other) const {
  return id == other.id;
}

bool ImageHandle::operator!=(const ImageHandle& other) const {
  return id != other.id;
}
