#pragma once

#include <memory>

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
  int MowementSpeed;

  virtual ~WorldTile();
  const std::string& TerrainTypeName() const;
  void SetDecoration(std::unique_ptr<WorldTileDecoration>);
  void SetResource(std::unique_ptr<WorldTileResource>);
  const WorldTileDecoration* Decoration() const;
  const WorldTileResource* Resource() const;
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
  std::unique_ptr<WorldTileDecoration> decoration;
  std::unique_ptr<WorldTileResource> resource;
};
