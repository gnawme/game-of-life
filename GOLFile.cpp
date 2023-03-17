/// \file GOLFile.cpp
#include "GOLFile.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>

namespace gol {

///
GOLFile::GOLFile(const char* filename)
        : m_filename(filename), m_extension(getExtension({filename})) {}

///
GOLFile::GOLFile(const std::string& filename)
        : m_filename(filename), m_extension(getExtension(filename)) {}

///
std::string GOLFile::getFilename() const {
    return m_filename;
}

///
PatternArray GOLFile::getPatternArray() {

    if (m_extension == CELLS) {
        return readPlaintextPatternFile(m_filename);
    } else {
        return readRLEPatternFile(m_filename);
    }
}

///
PatternArray GOLFile::readPatternFile(const char* filename) {
    m_filename = {filename};
    m_extension = getExtension(m_filename);

    return getPatternArray();
}

///
PatternArray GOLFile::readPatternFile(const std::string& filename) {
    m_filename = filename;
    m_extension = getExtension(m_filename);

    return getPatternArray();
}

/// \note PRIVATE
std::string GOLFile::getExtension(const std::string& patternFileName) {
    auto dot = patternFileName.find_last_of('.');
    if (dot != std::string::npos) {
        return {patternFileName.substr(dot + 1)};
    }

    return {};
}

/// \note PRIVATE
PatternArray GOLFile::readPlaintextPatternFile(const std::string& filename) {
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
                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
                grid.push_back(line);
            }
        }

        return grid;
    }

    return {};
}

/// \note PRIVATE
PatternArray GOLFile::readRLEPatternFile(const std::string& filename) {
    return {};
}
}  // namespace gol