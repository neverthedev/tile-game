# TASK-10: Improve Input Handling Pattern

**Priority:** 🟢 Medium
**Status:** 📋 TODO
**Estimated Effort:** Low

---

## Problem Description

Input handling breaks on first collision, preventing input pass-through:

```cpp
void GameInterface::HandleInput() {
  Vector2 mouse = GetMousePosition();

  for(int i = gameAreas.size() - 1; i >= 0; --i) {
    GameArea* gameArea = gameAreas[i];

    if (CheckCollisionPointRec(mouse, rect)) {
      gameArea->HandleInput();
      break;  // Stops checking other areas!
    }
  }
}
```

### Why This Is Wrong

1. **No Input Pass-Through**: Can't have transparent UI
2. **Binary Consumption**: Either handles or doesn't, no nuance
3. **Rigid Behavior**: Can't implement click-through elements
4. **Tooltip Issues**: Can't show tooltips over other elements
5. **Modal Dialogs Hard**: Can't properly block input to background

---

## Desired Outcome

Flexible input system with explicit consumption.

### Return Bool for Consumption

```cpp
class IInputHandler {
  public:
    // Returns true if input was consumed, false to pass through
    virtual bool HandleInput() = 0;
};

void GameInterface::HandleInput() {
  Vector2 mouse = GetMousePosition();

  for(int i = gameAreas.size() - 1; i >= 0; --i) {
    GameArea* gameArea = gameAreas[i];

    if (CheckCollisionPointRec(mouse, gameArea->GetBounds())) {
      bool consumed = gameArea->HandleInput();
      if (consumed) {
        break;  // Stop only if consumed
      }
      // Otherwise continue to next area
    }
  }
}
```

### Input Event System

```cpp
enum class InputEventType {
  MOUSE_CLICK,
  MOUSE_MOVE,
  KEY_PRESS,
  MOUSE_SCROLL
};

struct InputEvent {
  InputEventType type;
  Vector2 position;
  int button;  // or key code
  bool consumed = false;
};

class IInputHandler {
  public:
    virtual void HandleInput(InputEvent& event) = 0;
};

// In handler
void MenuButton::HandleInput(InputEvent& event) {
  if (event.type == InputEventType::MOUSE_CLICK &&
      Contains(event.position)) {
    OnClick();
    event.consumed = true;  // Mark as consumed
  }
}
```

### Input Blocking

```cpp
class GameArea {
  public:
    bool IsInputBlocking() const { return inputBlocking_; }
    void SetInputBlocking(bool blocking) { inputBlocking_ = blocking; }

  private:
    bool inputBlocking_ = true;  // Modal dialogs block
};

void GameInterface::HandleInput() {
  for (auto& area : areasReversed) {
    if (area->Contains(mouse)) {
      area->HandleInput();

      if (area->IsInputBlocking()) {
        break;  // Modal blocks further input
      }
    }
  }
}
```

---

## Implementation Steps

1. Update IInputHandler to return bool
2. Update all HandleInput implementations
3. Modify GameInterface to respect consumption
4. Add input event struct
5. Add blocking flag to GameArea
6. Test pass-through scenarios

---

## Files Affected

- `src/common/game_object.cpp` - IInputHandler interface
- `src/game_interface_.cpp` - Input dispatch logic
- `src/common/game_area.cpp` - Add blocking flag
- All HandleInput implementations

---

## Benefits

✅ Flexible input routing
✅ Transparent UI elements
✅ Proper modal dialogs
✅ Tooltip support
✅ Click-through areas
✅ Better UX control
