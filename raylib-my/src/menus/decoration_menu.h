#pragma once

#include <memory>
#include <string>
#include <vector>

#include "menu.h"

class DecorationMenu: public Menu {
  friend class MenuFactory;

public:
  DecorationMenu(
    Rectangle2D,
    std::unique_ptr<InputComponent>,
    std::unique_ptr<UpdateComponent>,
    std::unique_ptr<GraphicsComponent>
  );

  const std::vector<std::string>& Items() const;
  int HoveredIndex() const;
  int SelectedIndex() const;
  void SetHoveredIndex(int);
  void SetSelectedIndex(int);
  Rectangle2D ItemRect(int) const;

private:
  static constexpr float itemHeight = 50.0f;
  static constexpr float itemSpacing = 3.0f;
  static constexpr float itemInsetX = 4.0f;
  static constexpr float itemInsetY = 4.0f;
  bool IsIndexValid(int) const;

  std::vector<std::string> items;
  int hoveredIndex;
  int selectedIndex;
};
