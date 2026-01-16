# TASK-11: Add Missing Abstraction Layers

**Priority:** 🟢 Medium
**Status:** 📋 TODO
**Estimated Effort:** Medium-High

---

## Problem Description

Graphics class does too much:
- Window management
- Rendering
- Coordinate conversions
- Texture loading
- Camera operations

No clear separation of responsibilities.

---

## Desired Outcome

Split into focused classes with single responsibilities.

### Proposed Architecture

```cpp
// Coordinate system transformations
class CoordinateSystem {
  public:
    Position2D GridToScreen(Position2D grid) const;
    Position2D ScreenToGrid(Position2D screen) const;
    Position2D ScreenToWorld(Position2D screen, Camera2D camera) const;
    Position2D WorldToScreen(Position2D world, Camera2D camera) const;
};

// Texture management
class TextureManager {
  public:
    Texture2D Load(const std::string& path);
    void Unload(const std::string& path);
    Texture2D Get(const std::string& path);

  private:
    std::unordered_map<std::string, Texture2D> textures_;
};

// Rendering operations
class Renderer {
  public:
    void BeginDrawing();
    void EndDrawing();
    void DrawTexture(Texture2D tex, Position2D pos, Color tint);
    void DrawRectangle(Rectangle2D rect, Color color);
    void DrawIsometricTile(Position2D pos, Texture2D tex);
};

// Updated Graphics as coordinator
class Graphics {
  public:
    Renderer& GetRenderer() { return renderer_; }
    TextureManager& GetTextures() { return textures_; }
    CoordinateSystem& GetCoordinates() { return coordinates_; }

  private:
    Renderer renderer_;
    TextureManager textures_;
    CoordinateSystem coordinates_;
};
```

---

## Benefits

✅ Single Responsibility Principle
✅ Easier to test
✅ More maintainable
✅ Reusable components
✅ Clearer interfaces
