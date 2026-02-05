#include "tile.h"
#include "../input_components/tile_component.h"
#include "../update_components/tile_component.h"
#include "../graphics_components/tile_component.h"

WorldTile::WorldTile(
  const WorldTileTerrainType& type,
  Position2D pos,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<UpdateComponent> upd,
  std::unique_ptr<GraphicsComponent> grph
):
  GameObject(std::move(inp), std::move(grph), std::move(upd)),
  Pos { pos },
  TerrainType { type },
  Dirty { true },
  decoration { nullptr },
  resource { nullptr },
  MowementSpeed { 0 }
{}

TextureHandle WorldTile::Texture() const {
  return TerrainType.Texture();
}

ImageHandle WorldTile::TextureImage() const {
  return TerrainType.TextureImage();
}

const std::string& WorldTile::TerrainTypeName() const {
  return TerrainType.Name();
}

void WorldTile::SetDecoration(std::unique_ptr<WorldTileDecoration> newDecoration) {
  decoration = std::move(newDecoration);
}

void WorldTile::SetResource(std::unique_ptr<WorldTileResource> newResource) {
  resource = std::move(newResource);
}

const WorldTileDecoration* WorldTile::Decoration() const {
  return decoration.get();
}

const WorldTileResource* WorldTile::Resource() const {
  return resource.get();
}

WorldTile::~WorldTile() {}

WorldTile* WorldTileTerrainType::NewTile(Position2D pos) const {
  return new WorldTile(
    *this,
    pos,
    std::make_unique<TileInputComponent>(),
    std::make_unique<TileUpdateComponent>(),
    std::make_unique<TileGraphicsComponent>()
  );
}
