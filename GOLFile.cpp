/// \file GOLFile.cpp
#include "GOLFile.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <string>

namespace gol {

///
GOLFile::GOLFile(const char* filename)
    : m_filename(filename)
    , m_extension(getExtension({filename})) {}

///
GOLFile::GOLFile(const std::string& filename)
    : m_filename(filename)
    , m_extension(getExtension(filename)) {}

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
    m_filename = filename;
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
std::size_t GOLFile::prescanPlaintext(std::ifstream& pattern) {
    std::size_t maxLineLen = std::numeric_limits<std::size_t>::min();
    std::string line;
    while (std::getline(pattern, line)) {
        if (line[0] == DELIM) {
            continue;
        }

        if (line.length() > maxLineLen) {
            maxLineLen = line.length();
        }
    }

    pattern.clear();
    pattern.seekg(0);

    return maxLineLen;
}

/// \note PRIVATE
/// \see https://conwaylife.com/wiki/Plaintext
PatternArray GOLFile::readPlaintextPatternFile(const std::string& filename) {
    std::ifstream pattern(filename);
    if (pattern) {
        auto maxLineLen = prescanPlaintext(pattern);
        PatternArray grid;
        std::string line;
        while (std::getline(pattern, line)) {
            if (line[0] == DELIM) {
                continue;
            }

            if (!line.empty()) {
                line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
            }

            // Some variations of the plaintext format exists, which were results of
            // older manual encoding and should be corrected. For example, trailing
            // dead cells are sometimes omitted on a line, and lines containing only
            // dead cells are sometimes left completely blank.
            if (line.length() < maxLineLen) {
                std::string pad(maxLineLen - line.length(), PTEXT_DEAD);
                line.append(pad);
            }

            grid.push_back(line);
        }

        return grid;
    }

    return {};
}

/// \note PRIVATE
PatternArray GOLFile::readRLEPatternFile(const std::string& filename) {
    std::ifstream pattern(filename);
    return {};
}
}  // namespace gol