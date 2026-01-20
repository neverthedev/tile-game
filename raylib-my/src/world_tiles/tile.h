#pragma once

#include "../common/position_2d.h"
#include "../common/game_object.h"
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
  TextureHandle Texture() const;
  ImageHandle TextureImage() const;

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
