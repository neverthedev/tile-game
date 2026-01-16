# TASK-13: Optimize gameAreas Sorting

**Priority:** 🔵 Low
**Status:** 📋 TODO
**Estimated Effort:** Low

---

## Problem Description

Sorting happens every time an area is added:

```cpp
void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  GameArea* area = new GameArea(obj, pos, priority);
  gameAreas.push_back(area);
  std::sort(gameAreas.begin(), gameAreas.end(), [](const GameArea* a1, const GameArea* a2) {
    return a1->priority < a2->priority;
  });
}
```

### Performance Impact

- O(n log n) on every addition
- Usually areas added during initialization only
- Low priority unless areas added frequently

---

## Desired Outcome

More efficient approach for maintaining sorted order.

### Option 1: Sorted Insertion

```cpp
void GameInterface::AddArea(GameObject& obj, Rectangle2D pos, int priority) {
  auto area = std::make_unique<GameArea>(obj, pos, priority);

  auto it = std::lower_bound(gameAreas.begin(), gameAreas.end(), area,
    [](const auto& a1, const auto& a2) {
      return a1->priority < a2->priority;
    });

  gameAreas.insert(it, std::move(area));
}
```

### Option 2: Priority Queue

```cpp
struct GameAreaComparator {
  bool operator()(const GameArea* a, const GameArea* b) const {
    return a->priority > b->priority;  // Min heap
  }
};

std::priority_queue<GameArea*, std::vector<GameArea*>, GameAreaComparator> gameAreas;
```

### Option 3: Sort Once

```cpp
void GameInterface::FinishSetup() {
  std::sort(gameAreas.begin(), gameAreas.end(), /* ... */);
  setupComplete_ = true;
}
```

---

## Benefits

✅ Better performance
✅ O(n) instead of O(n log n)
✅ Cleaner code
