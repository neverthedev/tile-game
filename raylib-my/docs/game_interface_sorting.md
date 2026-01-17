# GameInterface - Index-Based Priority Sorting

## Overview

The `GameInterface` class manages multiple `GameArea` objects with different priority levels. To handle input correctly, areas must be processed in priority order (highest priority first), but the `GameArea` objects cannot be moved after construction due to their reference members.

## Solution: Index-Based Sorting

Instead of sorting the `GameArea` objects themselves, we maintain a separate vector of indices that tracks the priority-sorted order.

### Data Structure

```cpp
class GameInterface {
private:
  std::vector<GameArea> gameAreas;        // Stores actual objects (never reordered)
  std::vector<size_t> sortedIndices;      // Indices in priority order
};
```

### How It Works

1. **Adding Areas**: When a new area is added via `AddArea()`:
   - Object is emplaced into `gameAreas` (no copying, no moving after construction)
   - Index is added to `sortedIndices`
   - Indices are sorted by comparing priorities: `gameAreas[i1].priority < gameAreas[i2].priority`

2. **Iteration**: All iterations go through `sortedIndices`:
   ```cpp
   for(size_t idx : sortedIndices) {
     gameAreas[idx].Update();  // Access in priority order
   }
   ```

3. **Reverse Priority**: HandleInput iterates in reverse (highest priority first):
   ```cpp
   for(int i = sortedIndices.size() - 1; i >= 0; --i) {
     GameArea& gameArea = gameAreas[sortedIndices[i]];
     // Highest priority area checked first
   }
   ```

## Why This Approach?

### Problem with Direct Sorting
`GameArea` contains a reference member (`GameObject& object`):
- References cannot be reassigned after initialization
- Move assignment operator would be broken: `object = other.object` doesn't rebind the reference
- `std::sort()` would corrupt the reference if it used move assignment

### Benefits of Index Sorting
✅ **GameArea objects never move** - references stay valid
✅ **No move assignment needed** - objects constructed in place and stay there
✅ **Clean separation** - storage vs. access order
✅ **Minimal overhead** - just extra `size_t` per area (8 bytes)
✅ **RAII compliant** - automatic cleanup, no manual memory management

## Memory Layout

```
gameAreas:       [GameArea_0] [GameArea_1] [GameArea_2]
                     ↑             ↑             ↑
                     |             |             |
priority:           5             2             8
                     |             |             |
sortedIndices:      [1,           0,            2]
                   (prio=2)    (prio=5)     (prio=8)
```

Iteration order: area[1] → area[0] → area[2]
Actual storage: Never changes after construction
