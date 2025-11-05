# game-of-life
C++ implementation of [Conway's Game of Life](https://conwaylife.com/wiki/Conway%27s_Game_of_Life) with [SFML](https://www.sfml-dev.org/index.php) graphics.

![Pulsar-like Oscillators](animations/osculator.png)

## Features

- Implemented using [SFML 3.0](https://github.com/SFML/SFML), which is standardized on C++17.
- Modernized with C++20 `std::filesystem` for robust path handling.
- Reads [plaintext](https://conwaylife.com/wiki/Plaintext) and [RLE](https://conwaylife.com/wiki/Run_Length_Encoded) pattern formats.
- Supports finite plane or 'infinite' grids via the `--wrapped` command line option.
- Supports random [soups](https://conwaylife.com/wiki/Soup#Soup_search) via the `--random` command line option.
- For pattern files, adaptively reduces the tiling size (from the nominal 16x16) in powers of 2 increments to support patterns up to 640x360.
- Colorizes the cell [transitions](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) rather than just showing live or dead.
- Cell colors can be configured via JSON
- Display parameters (screen size, tile size) can be configured via JSON.

### Unique Cell Transition Colorization

Unlike traditional Game of Life implementations that only distinguish between alive (on) and dead (off) cells, this implementation visualizes **how and why** cells change state. Each cell is colored based on its transition type:

| State | Color | Meaning |
|-------|-------|---------|
| **CELL_ASLEEP** | Blue (`0x003F5C`) | Dead cell (0-1 neighbors) |
| **CELL_LONELY** | Purple (`0x58508D`) | Just died from underpopulation (≤1 neighbor) |
| **CELL_CHOKED** | Pink (`0xBC5090`) | Just died from overpopulation (≥4 neighbors) |
| **CELL_LIVING** | Orange (`0xFFA600`) | Surviving cell (2-3 neighbors) |
| **CELL_REBORN** | Red (`0xFF6361`) | Just born from reproduction (exactly 3 neighbors) |

This creates a rich visualization where you can see:
- **Purple halos** around collapsing patterns (cells starving)
- **Pink flashes** in overcrowded regions (cells suffocating)
- **Red bursts** where new life emerges
- **Orange cores** in stable structures
- **Blue backgrounds** in lifeless regions

The colors are fully configurable via `gol_config.json`. This approach transforms the classic binary view into a temporal heat map of population dynamics.

## Gallery

Watch Conway's Game of Life in action with colorized cell transitions that reveal the dynamics of population change:

### Classic Patterns

#### Pulsar
![Pulsar Oscillator](animations/pulsar.gif)

The pulsar is a period-3 oscillator. Notice the **purple halos** (underpopulation deaths) and **red bursts** (births) that create its characteristic pulsing rhythm.

#### Gosper Glider Gun
![Gosper Glider Gun](animations/gosper_glider_gun.gif)

The first discovered pattern that grows indefinitely, continuously producing gliders. Watch the **orange stable core** while new gliders emerge in **red** at regular intervals.

### Random Soup
![Random Soup Evolution](animations/random_soup.gif)

A randomly generated initial configuration. Observe the chaotic interactions with **pink flashes** (overcrowding deaths), **purple regions** (starvation), and eventual stabilization or extinction.

---

*All animations show the unique colorization scheme where cell colors indicate their transition state rather than just alive/dead status. See the [features section](#unique-cell-transition-colorization) for the complete color legend.*

## Dependencies

This project uses a hybrid dependency management approach:

### Conan Dependencies
- **nlohmann_json** - JSON parsing for configuration files
- **GTest** - Unit testing framework

### CMake FetchContent Dependencies
- **SFML 3.0** - Graphics and windowing library
- **effolkronium/random** - Modern C++ random number generation
- **cxxopts** - Command-line argument parsing

## Building

### Prerequisites

**Ubuntu 24.04:**
```bash
# Install build tools
sudo apt install cmake build-essential

# Install pip and Conan
sudo apt install pipx
pipx ensurepath
pipx install conan

# Install SFML build dependencies
sudo apt install libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                 libudev-dev libgl1-mesa-dev libfreetype6-dev \
                 libopenal-dev libvorbis-dev libflac-dev
```

**macOS:**
```bash
# Install Homebrew (if not already installed)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install Conan
brew install conan
```

### Build Instructions

```bash
# 1. Clone the repository
git clone <repository-url>
cd game-of-life

# 2. Create Conan profile (one-time setup)
conan profile detect

# 3. Build the project
./build.sh

# 4. Run the executable
./build/game_of_life --help
```

### Build Script Options

The `build.sh` script supports several options:

```bash
# Build in debug mode
./build.sh --debug

# Use a custom build directory
./build.sh --build-dir my-build

# Specify number of parallel jobs
./build.sh --jobs 8
# or
./build.sh -j 8

# Clean build directory before building
./build.sh --clean

# Show help
./build.sh --help
```

### macOS Build Notes

Due to legacy CMake requirements in some dependencies, the build script automatically handles this with the `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` flag.

### Running Tests

```bash
./build/gol_unit_tests
```

### Development Build

For development with debug symbols:

```bash
# Build in debug mode
./build.sh --debug

# Run
./build/game_of_life [options]
```

## Usage

```
Conway's Game of Life - Modern C++ Implementation with SFML Graphics

USAGE:
  game_of_life <pattern-file> [OPTIONS]
  game_of_life --random [OPTIONS]

ARGUMENTS:
  <pattern-file>    Path to a pattern file (.cells or .rle format)
                    Example: ./patterns/glider.cells

OPTIONS:
  --random          Generate a random soup instead of loading a pattern
  --wrapped         Enable wrapped/toroidal grid (infinite plane)
                    Default: bounded grid with edges
  -h, --help        Display help message and exit

INTERACTIVE CONTROLS:
  F5                Toggle fullscreen mode
  ESC               Exit the simulation

EXAMPLES:
  # Run with a pattern file
  game_of_life patterns/glider.cells

  # Run with wrapped grid (infinite plane)
  game_of_life patterns/gosper_glider_gun.rle --wrapped

  # Generate a random soup
  game_of_life --random

  # Random soup with wrapped grid
  game_of_life --random --wrapped
```

## Configuration

Display parameters and cell colors can be customized via `gol_config.json` in the application directory.

Example configuration:
```json
{
  "displayParams": {
    "widthInPixels": "1280",
    "heightInPixels": "720",
    "tileSize": "16.0"
  },
  "stateColors": {
    "colorAsleep": "0x003F5CFF",
    "colorLonely": "0x58508DFF",
    "colorChoked": "0xBC5090FF",
    "colorLiving": "0xFFA600FF",
    "colorReborn": "0xFF6361FF"
  },
  "lifeTickInSecs": "0.5",
  "startupDelaySecs": "3.0"
}
```

## Pattern Files

This implementation supports two standard formats:

- **Plaintext (.cells)**: [Format specification](https://conwaylife.com/wiki/Plaintext)
- **RLE (.rle)**: [Format specification](https://conwaylife.com/wiki/Run_Length_Encoded)

Pattern files can be found at [LifeWiki](https://conwaylife.com/wiki/Main_Page).

## Project Structure

```
game-of-life/
├── CMakeLists.txt              # Root CMake configuration
├── conanfile.py                # Conan dependency management
├── build.sh                    # Build automation script
├── gol_config.json             # Runtime configuration
├── main.cpp                    # Main entry point
├── GameOfLife.cpp/.h           # Game engine
├── Window.cpp/.h               # SFML window wrapper
├── GOLFile.cpp/.h              # Pattern file parser
├── GOLConfig.cpp/.h            # Configuration loader
├── ConwayGrid.cpp/.h           # Grid data structure
├── MooreNeighbor.cpp/.h        # Neighbor calculation
├── ConwayDefs.h                # Type definitions
├── tests/                      # Unit tests
│   └── ...
└── patterns/                   # Sample pattern files
    ├── glider.cells
    ├── pulsar.cells
    └── gosper_glider_gun.rle
```

## License

MIT License - see source files for full license text.

## Acknowledgments

- [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) - John Horton Conway
- [SFML](https://www.sfml-dev.org/) - Simple and Fast Multimedia Library
- [LifeWiki](https://conwaylife.com/wiki/) - Pattern library and documentation