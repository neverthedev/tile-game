#include "game_config.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

using nlohmann::json;

namespace {
  const json& RequireObject(const json& j, const char* name) {
    if (!j.contains(name) || !j.at(name).is_object()) {
      throw std::runtime_error(std::string("Missing or invalid object: ") + name);
    }
    return j.at(name);
  }

  template <typename T>
  T RequireField(const json& j, const char* name) {
    if (!j.contains(name)) {
      throw std::runtime_error(std::string("Missing required field: ") + name);
    }
    try {
      return j.at(name).get<T>();
    } catch (const std::exception& ex) {
      throw std::runtime_error(std::string("Invalid field type: ") + name + ": " + ex.what());
    }
  }
}

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

  const json& display = RequireObject(j, "display");
  const json& world = RequireObject(j, "world");

  GameConfig config;
  config.ScreenWidth = RequireField<int>(display, "screenWidth");
  config.ScreenHeight = RequireField<int>(display, "screenHeight");
  config.FrameRate = RequireField<int>(display, "frameRate");
  config.Fullscreen = RequireField<bool>(display, "fullscreen");
  config.WindowTitle = RequireField<std::string>(display, "windowTitle");

  config.TileWidth = RequireField<float>(world, "tileWidth");
  config.TileHeight = RequireField<float>(world, "tileHeight");
  config.WorldWidth = RequireField<int>(world, "worldWidth");
  config.WorldHeight = RequireField<int>(world, "worldHeight");

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
