#include "tile_component.h"
#include "../world_tiles/tile.h"
#include "../graphics/render_system.h"
#include "../common/color_2d.h"

TileGraphicsComponent::TileGraphicsComponent() {}

void TileGraphicsComponent::Render(GameObject& obj, RenderSystem& renderer) {
  WorldTile& tile = static_cast<WorldTile&>(obj);
  tile.Dirty = false;

  // Draw isometric tile frame
  Position2D center = renderer.GridToScreen(tile.Pos);
  center += renderer.GetCorrection();

  ImageHandle textureImage = tile.TextureImage();
  float imgWidth = static_cast<float>(renderer.GetImageWidth(textureImage));
  float imgHeight = static_cast<float>(renderer.GetImageHeight(textureImage));
  Rectangle2D src = { 0, 0, imgWidth, imgHeight };
  Rectangle2D dst = { center.x - renderer.GetTileWidth() * 0.5f, center.y - renderer.GetTileHeight() * 0.5f, renderer.GetTileWidth(), renderer.GetTileHeight() };

  //DrawTexturePro(texture, src , dst, {0, 0}, 0, WHITE);
  renderer.ImageDraw(renderer.GetDst(), textureImage, src, dst, Color2D::White());
  renderer.DrawDiamondFrame(center, Color2D::Black(), true, 1.0f);
}

TileGraphicsComponent::~TileGraphicsComponent() {}
