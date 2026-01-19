#include "tile_component.h"
#include "../world_tiles/tile.h"
#include "../graphics.h"
#include "../common/color_2d.h"

TileGraphicsComponent::TileGraphicsComponent() {}

void TileGraphicsComponent::Render(GameObject& obj, Graphics& grph) {
  WorldTile& tile = static_cast<WorldTile&>(obj);
  tile.Dirty = false;

  // Draw isometric tile frame
  Position2D center = grph.GridToScreen(tile.Pos);
  center += grph.Correction;

  const Texture2D& texture = tile.Texture();
  Rectangle src = { 0, 0, (float) texture.width, (float) texture.height };
  Rectangle dst = { center.x - grph.TileWidth * 0.5f, center.y - grph.TileHeight * 0.5f, grph.TileWidth, grph.TileHeight };

  //DrawTexturePro(texture, src , dst, {0, 0}, 0, WHITE);
  ImageDraw(grph.Dst, tile.TextureImage(), src, dst, WHITE);
  grph.DrawDiamondFrame(center, Color2D::Black(), true, 1.0f);
}

TileGraphicsComponent::~TileGraphicsComponent() {}
