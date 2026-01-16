# TASK-14: Add Spatial Partitioning

**Priority:** 🔵 Low
**Status:** 📋 TODO
**Estimated Effort:** Medium-High

---

## Problem Description

All tiles potentially checked every frame:
- No culling for off-screen objects
- All tiles iterated for rendering
- No spatial optimization

### Performance Impact

Currently not an issue with 60x80 grid (4,800 tiles), but could be with:
- Larger worlds
- More entities
- Complex decorations

---

## Desired Outcome

Efficient spatial queries for large worlds.

### Quadtree Implementation

```cpp
class Quadtree {
  public:
    void Insert(WorldTile* tile, Rectangle2D bounds);
    void Remove(WorldTile* tile);
    std::vector<WorldTile*> Query(Rectangle2D area);
    void Clear();

  private:
    struct Node {
      Rectangle2D bounds;
      std::vector<WorldTile*> objects;
      std::unique_ptr<Node> children[4];

      void Subdivide();
      bool IsLeaf() const;
    };

    std::unique_ptr<Node> root_;
    int maxDepth_ = 8;
    int maxObjectsPerNode_ = 10;
};
```

### Usage in Rendering

```cpp
void GameWorld::Render(Graphics& graphics) {
  // Get visible area from camera
  Rectangle2D visibleArea = GetVisibleArea(graphics.camera);

  // Query only visible tiles
  auto visibleTiles = spatialIndex_.Query(visibleArea);

  // Render only those
  for (auto* tile : visibleTiles) {
    tile->Render(graphics);
  }
}
```

### Grid-Based Alternative (Simpler)

```cpp
class SpatialGrid {
  public:
    void Insert(WorldTile* tile);
    std::vector<WorldTile*> GetTilesInArea(Rectangle2D area);

  private:
    int cellSize_;
    std::unordered_map<Position2D, std::vector<WorldTile*>> grid_;
};
```

---

## Benefits

✅ Faster rendering
✅ Efficient collision queries
✅ Supports large worlds
✅ Better scalability

---

## When to Implement

- World size > 100x100
- Performance issues detected
- Adding collision detection
- Many dynamic entities
