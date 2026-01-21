#pragma once

#include <string>

class GameConfig {
public:
  // Display settings
  int ScreenWidth = 1024;
  int ScreenHeight = 768;
  int FrameRate = 60;
  bool Fullscreen = false;
  std::string WindowTitle = "IsoGame Test";

  // Tile settings
  float TileWidth = 64.0f;
  float TileHeight = 32.0f;

  // World settings
  int WorldWidth = 60;
  int WorldHeight = 80;

  static GameConfig LoadFromFile(const std::string& path);
  void SaveToFile(const std::string& path) const;

  static GameConfig LoadFromJson(const std::string& json_text);
  std::string ToJson() const;

private:
  void Validate() const;
};
