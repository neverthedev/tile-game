#pragma once

#include <memory>

class TilesManager;

class ServiceLocator {
public:
  static void Initialize();
  static void Shutdown();
  static void LoadResources(class RaylibGraphics& graphics);
  static TilesManager& GetTilesManager();

private:
  static std::unique_ptr<TilesManager> tilesManager;
};
