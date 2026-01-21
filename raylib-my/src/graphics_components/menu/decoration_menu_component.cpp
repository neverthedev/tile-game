#include "decoration_menu_component.h"

#include <algorithm>

#include "../../graphics/render_system.h"
#include "../../menus/decoration_menu.h"
#include "../../common/game_error.h"
#include "../../common/color_2d.h"
#include "../../services/service_locator.h"
#include "../../services/tiles_manager.h"

DecorationMenuGraphicsComponent::DecorationMenuGraphicsComponent(): GraphicsComponent() {}

void DecorationMenuGraphicsComponent::Render(GameObject& menu, RenderSystem& renderer) {
  DecorationMenu *decorationMenu = dynamic_cast<DecorationMenu*>(&menu);
  if (!decorationMenu) throw GameError("Incorrect object type provided!");

  const Color2D menuBackground = Color2D::MenuBackground();
  const Color2D rowBackground = Color2D::MenuRowBackground();
  const Color2D rowSelected = Color2D::MenuRowSelected();
  const Color2D rowHoverBorder = Color2D::MenuRowHoverBorder();
  const Color2D textColor = Color2D::MenuText();

  renderer.DrawRectangle(decorationMenu->Position, menuBackground);

  const auto& items = decorationMenu->Items();
  const int itemCount = static_cast<int>(items.size());
  TilesManager& tilesManager = ServiceLocator::GetTilesManager();
  const auto& tileTypes = tilesManager.TileTypes();

  for (int i = 0; i < itemCount; ++i) {
    Rectangle2D rowRect = decorationMenu->ItemRect(i);
    bool isSelected = (i == decorationMenu->SelectedIndex());
    bool isHovered = (i == decorationMenu->HoveredIndex());
    float border = 1.0f;

    if (isHovered) {
      renderer.DrawRectangle(rowRect, rowHoverBorder);
    }

    Rectangle2D fillRect = rowRect;
    if (isHovered) {
      fillRect = {
        rowRect.x + border,
        rowRect.y + border,
        rowRect.width - 2.0f * border,
        rowRect.height - 2.0f * border
      };
    }

    renderer.DrawRectangle(fillRect, isSelected ? rowSelected : rowBackground);

    auto it = tileTypes.find(items[i]);
    if (it == tileTypes.end()) {
      continue;
    }

    const WorldTileTerrainType& tileType = it->second;
    ImageHandle imgHandle = tileType.TextureImage();
    int imgW = renderer.GetImageWidth(imgHandle);
    int imgH = renderer.GetImageHeight(imgHandle);
    float imgX = rowRect.x + 6.0f;
    float imgY = rowRect.y + (rowRect.height - static_cast<float>(imgH)) * 0.5f;
    float maxThumbSize = rowRect.height - 12.0f;
    float maxDim = static_cast<float>(std::max(imgW, imgH));
    float scale = maxDim > 0.0f ? (maxThumbSize / maxDim) : 1.0f;
    if (scale > 1.0f) {
      scale = 1.0f;
    }
    float scaledW = static_cast<float>(imgW) * scale;
    float scaledH = static_cast<float>(imgH) * scale;
    imgY = rowRect.y + (rowRect.height - scaledH) * 0.5f;
    renderer.DrawTexture(tileType.Texture(), { imgX, imgY }, Color2D::White(), scale);

    float textX = imgX + scaledW + 10.0f;
    int fontSize = 16;
    float textY = rowRect.y + (rowRect.height - fontSize) * 0.5f;
    renderer.DrawText(items[i].c_str(), { textX, textY }, fontSize, textColor);
  }
}

DecorationMenuGraphicsComponent::~DecorationMenuGraphicsComponent() {}
