#pragma once

#include "../common/position_2d.h"
#include "../common/game_object.h"
#include "../graphics.h"
#include "tile_terrain_type.h"
#include "decorations/decoration.h"
#include "resources/resource.h"

class WorldTile: GameObject {
  friend class WorldTileTerrainType;

public:
  bool Dirty;
  Position2D Pos;
  WorldTileDecoration* Decoration;
  WorldTileResource* Resource;
  int MowementSpeed;

  virtual void HandleInput() override;
  virtual void Update() override;
  virtual void Render(Graphics&) override;
  virtual ~WorldTile();
  const Texture2D& Texture() const;
  const Image& TextureImage() const;

private:
  WorldTile(const WorldTileTerrainType&, Position2D);
  const WorldTileTerrainType& TerrainType;
  void drawIsoTileFrame(Graphics&);
};
