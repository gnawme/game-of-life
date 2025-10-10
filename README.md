# game-of-life
C++ implementation of [Conway's Game of Life](https://conwaylife.com/wiki/Conway%27s_Game_of_Life) with [SFML](https://www.sfml-dev.org/index.php) graphics.

## Features

- Implemented using [SFML 3.0](https://github.com/SFML/SFML), which is standardized on C++17.
- This version uses `std::filesystem`, which is available in C++17 through `std=c++20a` on gcc.
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

**Example:** In the pulsar oscillator (shown below), you can see the characteristic purple underpopulation deaths and red births that create the pulsing rhythm, rather than just cells turning on and off.

![Colorized Pulsar pattern](pulsar.png "Colorized pulsar")

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

# macOS
brew install conan
conan profile detect

# Install SFML build dependencies
sudo apt install libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
                 libudev-dev libgl1-mesa-dev libfreetype6-dev \
                 libopenal-dev libvorbis-dev libflac-dev

# 1. Clone the repository
git clone <repository-url>
cd game-of-life

# 2. Install Conan dependencies
conan install . --output-folder=build --build=missing

# 3. Configure CMake with Conan toolchain
cmake -B build -DCMAKE_TOOLCHAIN_FILE=build/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

# 4. Build the project
cmake --build build -j$(nproc)

# 5. Run the executable
./build/game_of_life [options]

# 6. Run tests
./build/gol_unit_tests
