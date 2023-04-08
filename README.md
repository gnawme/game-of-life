# game-of-life
C++ implementation of [Conway's Game of Life](https://conwaylife.com/wiki/Conway%27s_Game_of_Life) with [SFML](https://www.sfml-dev.org/index.php) graphics.

- Implemented using [SFML 3.0](https://github.com/SFML/SFML), which is standardized on C++17.
- Reads [plaintext](https://conwaylife.com/wiki/Plaintext) and [RLE](https://conwaylife.com/wiki/Run_Length_Encoded) pattern formats.
- Supports finite plane or 'infinite' grids via the `--wrapped` command line option.
- Supports random [soups](https://conwaylife.com/wiki/Soup#Soup_search) via the `--random` command line option.
- For pattern files, adaptively reduces the tiling size (from the nominal 16x16) in powers of 2 increments to support patterns up to 640x360.
- Colorizes the cell [transitions](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life) rather than just showing live or dead.
- Cell colors can be configured via JSON
- Display parameters (screen size, tile size) can be configured via JSON.

![Colorized Pulsar pattern](pulsar.png "Colorized pulsar")
