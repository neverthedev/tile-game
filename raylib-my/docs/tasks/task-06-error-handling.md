# TASK-06: Define Error Handling Strategy

**Priority:** 🟡 High
**Status:** 📋 TODO
**Estimated Effort:** Medium

---

## Problem Description

The project has a `GameError` class but no clear error handling strategy:

```cpp
#include "common/game_error.cpp"
```

Questions without clear answers:
- When should exceptions be used vs error codes?
- What errors are recoverable?
- How should errors be propagated?
- What happens when loading a texture fails?
- How are errors logged?
- What's the recovery strategy?

### Why This Is Wrong

1. **Inconsistent Error Handling**: No documented approach
2. **Silent Failures**: Errors might go unnoticed
3. **Unclear Recovery**: Don't know what to do when errors occur
4. **No Error Boundaries**: Crashes may occur instead of graceful handling
5. **Poor User Experience**: No error messages or recovery options

### Current Impact

Potential issues:
- Crashes on missing resources
- Undefined behavior on errors
- Difficult debugging
- No error reporting to users
- Impossible to write robust error handling

---

## Desired Outcome

Establish a clear, documented error handling strategy with concrete implementations.

### Error Categories

Define categories based on recoverability:

```cpp
// src/common/error_category.h
enum class ErrorCategory {
  CRITICAL,      // Unrecoverable, must terminate
  RECOVERABLE,   // Can continue with degraded functionality
  WARNING,       // Non-critical, log and continue
  INFO          // Informational only
};
```

### Error Handling Strategy

```cpp
// src/common/game_error.h
class GameError : public std::runtime_error {
  public:
    GameError(ErrorCategory category,
              const std::string& message,
              const std::string& context = "")
      : std::runtime_error(message),
        category_(category),
        context_(context) {}

    ErrorCategory GetCategory() const { return category_; }
    const std::string& GetContext() const { return context_; }

  private:
    ErrorCategory category_;
    std::string context_;
};

// Specific error types
class ResourceLoadError : public GameError {
  public:
    ResourceLoadError(const std::string& resource)
      : GameError(ErrorCategory::RECOVERABLE,
                  "Failed to load resource: " + resource,
                  "Resource Loading") {}
};

class InvalidStateError : public GameError {
  public:
    InvalidStateError(const std::string& state)
      : GameError(ErrorCategory::CRITICAL,
                  "Invalid state: " + state,
                  "Game State") {}
};
```

---

## Implementation Steps

### 1. Define Error Types

Create `src/common/error_types.h`:

```cpp
#ifndef ERROR_TYPES_H
#define ERROR_TYPES_H

#include <exception>
#include <string>
#include <optional>

// Category enum
enum class ErrorCategory {
  CRITICAL,      // Terminate application
  RECOVERABLE,   // Continue with fallback
  WARNING,       // Log and continue
  INFO          // Informational
};

// Base error class
class GameError : public std::runtime_error {
  public:
    GameError(ErrorCategory category,
              const std::string& message,
              const std::string& context = "");

    ErrorCategory GetCategory() const;
    const std::string& GetContext() const;
    std::string GetFullMessage() const;

  private:
    ErrorCategory category_;
    std::string context_;
};

// Specific error types
class ResourceLoadError : public GameError { /* ... */ };
class FileNotFoundError : public GameError { /* ... */ };
class InvalidConfigError : public GameError { /* ... */ };
class RenderError : public GameError { /* ... */ };
class InvalidStateError : public GameError { /* ... */ };

#endif
```

### 2. Create Error Handler

```cpp
// src/common/error_handler.h
class ErrorHandler {
  public:
    static ErrorHandler& Instance();

    void Handle(const GameError& error);
    void Log(const std::string& message, ErrorCategory category);
    void SetLogFile(const std::string& path);

    // Callbacks for different error categories
    using ErrorCallback = std::function<void(const GameError&)>;
    void SetCriticalHandler(ErrorCallback handler);
    void SetRecoverableHandler(ErrorCallback handler);

  private:
    ErrorHandler() = default;
    std::ofstream logFile_;
    ErrorCallback criticalHandler_;
    ErrorCallback recoverableHandler_;
};
```

### 3. Use Result Type for Recoverable Errors

```cpp
// src/common/result.h
template<typename T, typename E = GameError>
class Result {
  public:
    static Result Success(T value) {
      Result r;
      r.value_ = std::move(value);
      return r;
    }

    static Result Failure(E error) {
      Result r;
      r.error_ = std::move(error);
      return r;
    }

    bool IsSuccess() const { return value_.has_value(); }
    bool IsFailure() const { return error_.has_value(); }

    const T& Value() const { return *value_; }
    const E& Error() const { return *error_; }

    // Monadic operations
    template<typename F>
    auto Map(F&& f) -> Result<decltype(f(*value_)), E>;

    template<typename F>
    auto AndThen(F&& f) -> decltype(f(*value_));

  private:
    std::optional<T> value_;
    std::optional<E> error_;
};
```

### 4. Apply to Resource Loading

```cpp
// Before
Texture2D LoadTexture(const char* filename) {
  return ::LoadTexture(filename);  // May fail silently
}

// After
Result<Texture2D, ResourceLoadError> LoadTexture(const char* filename) {
  Texture2D texture = ::LoadTexture(filename);

  if (texture.id == 0) {
    return Result<Texture2D, ResourceLoadError>::Failure(
      ResourceLoadError(filename)
    );
  }

  return Result<Texture2D, ResourceLoadError>::Success(texture);
}

// Usage
auto result = graphics.LoadTexture("player.png");
if (result.IsSuccess()) {
  Texture2D texture = result.Value();
  // Use texture
} else {
  // Handle error
  ErrorHandler::Instance().Handle(result.Error());
  // Use fallback texture
  texture = fallbackTexture;
}
```

### 5. Error Boundaries in Main Loop

```cpp
int main() {
  try {
    Graphics graphics(...);
    GameInterface interface(...);

    // Setup error handlers
    ErrorHandler::Instance().SetCriticalHandler([](const GameError& e) {
      std::cerr << "CRITICAL ERROR: " << e.GetFullMessage() << std::endl;
      // Show error dialog to user
      // Cleanup and exit
      exit(1);
    });

    ErrorHandler::Instance().SetRecoverableHandler([](const GameError& e) {
      std::cerr << "ERROR: " << e.GetFullMessage() << std::endl;
      // Log error, continue with degraded functionality
    });

    // Game loop
    while (!graphics.Done()) {
      try {
        interface.HandleInput();
        interface.Update();
        interface.Render(graphics);
      } catch (const GameError& e) {
        ErrorHandler::Instance().Handle(e);
        if (e.GetCategory() == ErrorCategory::CRITICAL) {
          break;  // Exit game loop
        }
        // Otherwise continue
      }
    }

  } catch (const std::exception& e) {
    std::cerr << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
```

---

## Error Handling Guidelines

### When to Use Exceptions

**Use exceptions for:**
- Resource loading failures
- Invalid configuration
- Unexpected states
- Initialization failures

```cpp
if (!worldData.IsValid()) {
  throw InvalidStateError("World data corrupted");
}
```

### When to Use Result Types

**Use Result<T> for:**
- Expected failure conditions
- Frequent operations
- Performance-critical code
- Functions that often fail

```cpp
Result<Tile*, NotFoundError> FindTile(Position2D pos);
```

### When to Use Error Codes

**Use error codes for:**
- C-style APIs
- Performance-critical inner loops
- When exceptions disabled

```cpp
enum class LoadResult {
  SUCCESS,
  FILE_NOT_FOUND,
  PARSE_ERROR,
  OUT_OF_MEMORY
};
```

---

## Files Affected

- `src/common/game_error.cpp` → Expand to full error system
- `src/graphics.cpp` → Add error handling to loading
- `src/managers/tiles_manager.cpp` → Handle load failures
- `src/game.cpp` → Add error boundaries
- All files that can fail

---

## Benefits After Completion

✅ Clear error handling strategy
✅ Recoverable from errors
✅ Better user experience
✅ Easier debugging
✅ Graceful degradation
✅ Logged errors for analysis
✅ Consistent error patterns

---

## Testing Approach

### Error Simulation Tests

```cpp
TEST(ResourceLoading, HandlesFileNotFound) {
  auto result = LoadTexture("nonexistent.png");
  EXPECT_TRUE(result.IsFailure());
  EXPECT_EQ(result.Error().GetCategory(), ErrorCategory::RECOVERABLE);
}

TEST(ErrorHandler, CriticalErrorTerminates) {
  bool handlerCalled = false;
  ErrorHandler::Instance().SetCriticalHandler([&](const GameError&) {
    handlerCalled = true;
  });

  InvalidStateError error("test");
  ErrorHandler::Instance().Handle(error);

  EXPECT_TRUE(handlerCalled);
}
```

---

## References

- [C++ Error Handling Best Practices](https://isocpp.org/wiki/faq/exceptions)
- [Result Type Pattern](https://doc.rust-lang.org/std/result/)
- [C++ Core Guidelines - E.2](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Re-throw)
