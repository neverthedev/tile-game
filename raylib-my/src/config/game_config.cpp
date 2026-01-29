#include "game_config.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "../common/json_require.h"

using nlohmann::json;

GameConfig GameConfig::LoadFromFile(const std::string& path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to open config file: " + path);
  }

  std::stringstream buffer;
  buffer << file.rdbuf();
  return LoadFromJson(buffer.str());
}

void GameConfig::SaveToFile(const std::string& path) const {
  std::ofstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to write config file: " + path);
  }
  file << ToJson();
}

GameConfig GameConfig::LoadFromJson(const std::string& json_text) {
  json j;
  try {
    j = json::parse(json_text);
  } catch (const std::exception& ex) {
    throw std::runtime_error(std::string("Failed to parse config JSON: ") + ex.what());
  }

  auto throw_runtime = [](const std::string& msg) {
    throw std::runtime_error(msg);
  };

  const json& display = JsonRequire::Object(j, "display", throw_runtime);
  const json& world = JsonRequire::Object(j, "world", throw_runtime);

  GameConfig config;
  config.ScreenWidth = JsonRequire::Field<int>(display, "screenWidth", throw_runtime);
  config.ScreenHeight = JsonRequire::Field<int>(display, "screenHeight", throw_runtime);
  config.FrameRate = JsonRequire::Field<int>(display, "frameRate", throw_runtime);
  config.Fullscreen = JsonRequire::Field<bool>(display, "fullscreen", throw_runtime);
  config.WindowTitle = JsonRequire::Field<std::string>(display, "windowTitle", throw_runtime);

  config.TileWidth = JsonRequire::Field<float>(world, "tileWidth", throw_runtime);
  config.TileHeight = JsonRequire::Field<float>(world, "tileHeight", throw_runtime);
  config.WorldWidth = JsonRequire::Field<int>(world, "worldWidth", throw_runtime);
  config.WorldHeight = JsonRequire::Field<int>(world, "worldHeight", throw_runtime);

  config.Validate();
  return config;
}

std::string GameConfig::ToJson() const {
  json j;
  j["display"] = {
    {"screenWidth", ScreenWidth},
    {"screenHeight", ScreenHeight},
    {"frameRate", FrameRate},
    {"fullscreen", Fullscreen},
    {"windowTitle", WindowTitle}
  };
  j["world"] = {
    {"tileWidth", TileWidth},
    {"tileHeight", TileHeight},
    {"worldWidth", WorldWidth},
    {"worldHeight", WorldHeight}
  };
  return j.dump(2);
}

void GameConfig::Validate() const {
  if (ScreenWidth <= 0 || ScreenHeight <= 0) {
    throw std::runtime_error("Screen dimensions must be positive.");
  }
  if (FrameRate <= 0) {
    throw std::runtime_error("Frame rate must be positive.");
  }
  if (WindowTitle.empty()) {
    throw std::runtime_error("Window title must not be empty.");
  }
  if (TileWidth <= 0.0f || TileHeight <= 0.0f) {
    throw std::runtime_error("Tile dimensions must be positive.");
  }
  if (WorldWidth <= 0 || WorldHeight <= 0) {
    throw std::runtime_error("World dimensions must be positive.");
  }
}
