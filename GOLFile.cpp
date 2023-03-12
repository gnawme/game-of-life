/// \file GOLFile.cpp
#include "GOLFile.h"

#include <fstream>
#include <string>

namespace gol {

///
PatternArray readPlaintextPatternFile(const char* filename) {
    constexpr char DELIM{'!'};

    std::ifstream pattern(filename);
    if (pattern) {
        PatternArray grid;
        std::string line;
        while (std::getline(pattern, line)) {
            if (line[0] == DELIM) {
                continue;
            }

            if (!line.empty()) {
                grid.push_back(line);
            }
        }

        return grid;
    }

    return {};
}
}  // namespace gol