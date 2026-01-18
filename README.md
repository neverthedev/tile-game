# Raylib-My: Isometric Game Engine

An isometric tile-based game engine built with C++ and raylib, featuring a component-based architecture and service locator pattern.

## Overview

This project is a 2D isometric game engine that demonstrates clean code organization, component-based design patterns, and modern C++ practices. The engine provides a flexible framework for building isometric tile-based games with camera controls, world management, and a modular UI system.

## Features

- **Isometric Tile Rendering** - Diamond-shaped isometric world with customizable tile dimensions
- **Component-Based Architecture** - Separation of input, graphics, and update logic through component composition
- **Camera System** - Pan and zoom controls for exploring the game world
- **Modular Menu System** - Extensible UI framework with decoration menus
- **Service Locator Pattern** - Clean dependency management for global services
- **Tile Management** - Efficient tile storage and retrieval system
- **Priority-Based Input Handling** - Area-based input with configurable priority levels

## Technologies

### Core Technologies
- **C++17** - Modern C++ standard with smart pointers and RAII
- **raylib 5.6-dev** - Simple and easy-to-use game development library
- **CMake 3.5+** - Cross-platform build system

### Development Tools
- **LLDB** - Debugging support
- **VS Code** - Primary development environment with CMake integration

## Project Structure

```
raylib-my/
├── CMakeLists.txt          # Build configuration
├── src/                    # Source code
│   ├── game.cpp           # Main entry point
│   ├── common/            # Core classes (GameObject, GameArea, etc.)
│   ├── managers/          # Game managers (TilesManager)
│   ├── menus/             # UI menu system
│   ├── services/          # Service locator and global services
│   ├── input_components/  # Input handling components
│   ├── graphics_components/ # Rendering components
│   ├── update_components/ # Game logic components
│   ├── world_tiles/       # Tile types and implementations
│   ├── game_world.cpp     # World management
│   ├── game_camera.cpp    # Camera system
│   ├── game_interface.cpp # UI and area management
│   └── graphics.cpp       # Graphics abstraction layer
├── textures/              # Game textures and sprites
└── docs/                  # Documentation
    ├── CODE_ORGANIZATION.md
    ├── component_architecture.md
    ├── SERVICE_LOCATOR.md
    └── tasks/            # Development tasks and improvements
```

## Getting Started

### Prerequisites

- CMake 3.5 or higher
- C++17 compatible compiler (GCC, Clang, MSVC)
- Git (for cloning raylib dependency)

### Building the Project

1. **Clone the repository** (if not already done):
   ```bash
   cd /path/to/game-test
   ```

2. **Configure the build**:
   ```bash
   cmake -S ./raylib-my -B ./raylib-my/build -DCMAKE_BUILD_TYPE=Debug
   ```

3. **Build the project**:
   ```bash
   cmake --build ./raylib-my/build
   ```

4. **Run the executable**:
   ```bash
   ./raylib-my/build/raylib-my/raylib-my
   ```

## Documentation

Comprehensive documentation is available in the `raylib-my/docs/` directory:

- **[CODE_ORGANIZATION.md](raylib-my/docs/CODE_ORGANIZATION.md)** - File structure and organization standards
- **[component_architecture.md](raylib-my/docs/component_architecture.md)** - Component system design
- **[SERVICE_LOCATOR.md](raylib-my/docs/SERVICE_LOCATOR.md)** - Service locator pattern implementation
- **[tasks/](raylib-my/docs/tasks/)** - Development tasks and improvement proposals

## Development Tasks

Active development tasks are tracked in `raylib-my/docs/tasks/`:

## References

### External Documentation
- [raylib Official Documentation](https://www.raylib.com/)
- [raylib Cheatsheet](https://www.raylib.com/cheatsheet/cheatsheet.html)
- [raylib GitHub Repository](https://github.com/raysan5/raylib)
- [Learn C++ Component-Based Architecture](https://gameprogrammingpatterns.com/component.html)

### Learning Resources
- [Isometric Game Math](https://clintbellanger.net/articles/isometric_math/)
- [Game Programming Patterns](https://gameprogrammingpatterns.com/)
- [raylib Examples](https://www.raylib.com/examples.html)

## License

This project uses raylib, which is licensed under the zlib/libpng license. See the [raylib LICENSE](raylib/LICENSE) for details.
