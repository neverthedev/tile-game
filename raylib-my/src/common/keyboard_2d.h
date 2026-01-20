#pragma once

// Lightweight keyboard constants for 2D input mapping.
// These mirror the numeric values used by raylib so higher-level
// code can avoid including raylib headers directly.
class Keyboard2D {
public:
  using Key = int;

  static constexpr Key KEY_RIGHT = 262;
  static constexpr Key KEY_LEFT  = 263;
  static constexpr Key KEY_DOWN  = 264;
  static constexpr Key KEY_UP    = 265;

  static constexpr Key KEY_A = 65;
  static constexpr Key KEY_D = 68;
  static constexpr Key KEY_S = 83;
  static constexpr Key KEY_W = 87;
};
