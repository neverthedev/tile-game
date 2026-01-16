# TASK-12: Improve Extensibility

**Priority:** 🟢 Medium
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

Limited extensibility:
- Only MenuFactory exists
- Hard to add new tile types
- No plugin system
- Difficult to add game modes

---

## Desired Outcome

More factory patterns and extensibility mechanisms.

### Tile Factory

```cpp
class TileFactory {
  public:
    using TileCreator = std::function<std::unique_ptr<WorldTile>()>;

    void RegisterTileType(const std::string& type, TileCreator creator);
    std::unique_ptr<WorldTile> CreateTile(const std::string& type);

  private:
    std::unordered_map<std::string, TileCreator> creators_;
};

// Register custom tiles
tileFactory.RegisterTileType("grass", []() {
  return std::make_unique<GrassTile>();
});
```

### Component Registry

```cpp
class ComponentRegistry {
  public:
    template<typename T>
    void Register(const std::string& name) {
      creators_[name] = []() { return std::make_unique<T>(); };
    }

    std::unique_ptr<IComponent> Create(const std::string& name);

  private:
    std::unordered_map<std::string, std::function<std::unique_ptr<IComponent>()>> creators_;
};
```

### Event System

```cpp
class EventSystem {
  public:
    using EventHandler = std::function<void(const Event&)>;

    void Subscribe(const std::string& eventType, EventHandler handler);
    void Publish(const std::string& eventType, const Event& event);

  private:
    std::unordered_map<std::string, std::vector<EventHandler>> handlers_;
};

// Usage
eventSystem.Subscribe("tile_clicked", [](const Event& e) {
  // Handle tile click
});

eventSystem.Publish("tile_clicked", TileClickedEvent{position});
```

---

## Benefits

✅ Easy to add new types
✅ Plugin architecture
✅ Loose coupling via events
✅ Modding support
✅ Data-driven design
