#include "tile.h"

WorldTile::WorldTile(const WorldTileTerrainType& type, Position2D pos):
  GameObject(),
  Pos { pos },
  TerrainType { type },
  Dirty { true }
{}

const Texture2D& WorldTile::Texture() const {
  return TerrainType.Texture();
}

const Image& WorldTile::TextureImage() const {
  return TerrainType.TextureImage();
}

void WorldTile::HandleInput() {

}

void WorldTile::Update() {
  //Dirty = true;
  // For example, grow the tree
}

void WorldTile::drawIsoTileFrame(Graphics& grph) {
  Position2D center = grph.GridToScreen(Pos);
  center += grph.Correction;

  const Texture2D& texture = Texture();
  Rectangle src = { 0, 0, (float) texture.width, (float) texture.height };
  Rectangle dst = { center.x - grph.TileWidth * 0.5f, center.y - grph.TileHeight * 0.5f, grph.TileWidth, grph.TileHeight };

  //DrawTexturePro(Texture(), src , dst, {0, 0}, 0, WHITE);
  ImageDraw(grph.Dst, TextureImage(), src, dst, WHITE);
  grph.DrawDiamondFrame(center, BLACK, true, 1.0f);
}

void WorldTile::Render(Graphics& grph) {
  Dirty = false;
  drawIsoTileFrame(grph);
}

WorldTile::~WorldTile() {}

WorldTile* WorldTileTerrainType::NewTile(Position2D pos) const {
  return new WorldTile(*this, pos);
}
