#include "factory.h"
#include "../services/service_locator.h"

#include "../input_components/menu/decoration_menu_component.h"
#include "../update_components/menu/decoration_menu_component.h"
#include "../graphics_components/menu/decoration_menu_component.h"

std::unique_ptr<DecorationMenu> MenuFactory::CreateDecorationMenu(Rectangle2D pos) {
  auto inp_cmp = std::make_unique<DecorationMenuInputComponent>();
  auto upd_cmp = std::make_unique<DecorationMenuUpdateComponent>();
  auto grph_cmp = std::make_unique<DecorationMenuGraphicsComponent>();

  return std::make_unique<DecorationMenu>(
    pos,
    std::move(inp_cmp),
    std::move(upd_cmp),
    std::move(grph_cmp)
  );
}
