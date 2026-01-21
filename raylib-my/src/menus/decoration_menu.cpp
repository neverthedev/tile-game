#include "decoration_menu.h"

#include <algorithm>

#include "../services/service_locator.h"
#include "../services/tiles_manager.h"

DecorationMenu::DecorationMenu(
  Rectangle2D pos,
  std::unique_ptr<InputComponent> inp,
  std::unique_ptr<UpdateComponent> upd,
  std::unique_ptr<GraphicsComponent> grph
):
  Menu(pos, std::move(inp), std::move(upd), std::move(grph)),
  hoveredIndex { -1 },
  selectedIndex { -1 }
{
  const TilesManager& tilesManager = ServiceLocator::GetTilesManager();
  items = tilesManager.TileTypeNames();
  std::sort(items.begin(), items.end());
}

const std::vector<std::string>& DecorationMenu::Items() const {
  return items;
}

int DecorationMenu::HoveredIndex() const {
  return hoveredIndex;
}

int DecorationMenu::SelectedIndex() const {
  return selectedIndex;
}

void DecorationMenu::SetHoveredIndex(int index) {
  hoveredIndex = IsIndexValid(index) ? index : -1;
}

void DecorationMenu::SetSelectedIndex(int index) {
  selectedIndex = IsIndexValid(index) ? index : -1;
}

Rectangle2D DecorationMenu::ItemRect(int index) const {
  float y = Position.y + itemInsetY + index * (itemHeight + itemSpacing);
  return {
    Position.x + itemInsetX,
    y,
    Position.width - 2.0f * itemInsetX,
    itemHeight
  };
}

bool DecorationMenu::IsIndexValid(int index) const {
  return index >= 0 && index < static_cast<int>(items.size());
}
