#ifndef WORLD_TILES_TILE
#define WORLD_TILES_TILE

#include "../common/position_2d.cpp"
#include "../common/game_object.cpp"
#include "../graphics.cpp"
#include "tile_terrain_type.cpp"
#include "decorations/decoration.cpp"
#include "resources/resource.cpp"

class WorldTile: GameObject { // Tile is a typed object of type TerrainType
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
};

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
};

WorldTile::~WorldTile() {}

WorldTile* WorldTileTerrainType::NewTile(Position2D pos) const {
  return new WorldTile(*this, pos);
}

#endif // WORLD_TILES_TILE
