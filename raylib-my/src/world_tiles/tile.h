#pragma once

#include "raylib.h"  // Include first to avoid conflicts with forward declarations
#include "../common/position_2d.h"
#include "../common/game_object.h"
#include "../graphics.h"
#include "tile_terrain_type.h"
#include "decorations/decoration.h"
#include "resources/resource.h"

class WorldTile: public GameObject {
  friend class WorldTileTerrainType;

public:
  bool Dirty;
  Position2D Pos;
  WorldTileDecoration* Decoration;
  WorldTileResource* Resource;
  int MowementSpeed;

  virtual ~WorldTile();
  const Texture2D& Texture() const;
  const Image& TextureImage() const;

private:
  WorldTile(
    const WorldTileTerrainType&,
    Position2D,
    std::unique_ptr<InputComponent>,
    std::unique_ptr<UpdateComponent>,
    std::unique_ptr<GraphicsComponent>
  );
  const WorldTileTerrainType& TerrainType;
};
