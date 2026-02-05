#pragma once

#include "../game_world.h"
#include "../services/tiles_manager.h"

#include "world_data_writer.h"

class WorldSaveService {
public:
  WorldSaveService(const TilesManager& tilesManager, WorldDataWriter& writer);
  void SaveWorld(const GameWorld& world);

private:
  const TilesManager& tilesManager;
  WorldDataWriter& writer;
};
