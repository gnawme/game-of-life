/// \file GOLFile.cpp
/// \author Norm Evangelista
/// \copyright (c) 2023 Norm Evangelista
// Copyright 2023 gnawme (Norm Evangelista)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "GOLFile.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
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
    namespace fs = std::filesystem;
    fs::path filePath(patternFileName);

    // extension() includes the dot, so remove it
    std::string ext = filePath.extension().string();
    if (!ext.empty() && ext[0] == '.') {
        return ext.substr(1);
    }
    return {};
}

/// \note PRIVATE
std::size_t GOLFile::prescanPlaintext(std::ifstream& pattern) {
    std::size_t maxLineLen = std::numeric_limits<std::size_t>::min();
    std::string line;
    while (std::getline(pattern, line)) {
        if (line[0] == PTEXT_DELIM) {
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
    namespace fs = std::filesystem;

    // Check file exists before attempting to open
    if (!fs::exists(filename)) {
        std::cerr << "Pattern file does not exist: " << filename << std::endl;
        return {};
    }

    // Check it's a regular file
    if (!fs::is_regular_file(filename)) {
        std::cerr << "Path is not a regular file: " << filename << std::endl;
        return {};
    }

    // Check file size
    auto fileSize = fs::file_size(filename);
    if (fileSize == 0) {
        std::cerr << "Pattern file is empty: " << filename << std::endl;
        return {};
    }

    std::ifstream pattern(filename);
    if (pattern) {
        auto maxLineLen = prescanPlaintext(pattern);
        PatternArray grid;
        std::string line;
        while (std::getline(pattern, line)) {
            if (line[0] == PTEXT_DELIM) {
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
/// \see https://conwaylife.com/wiki/Run_Length_Encoded
PatternArray GOLFile::readRLEPatternFile(const std::string& filename) {
    namespace fs = std::filesystem;

    // Check file exists before attempting to open
    if (!fs::exists(filename)) {
        std::cerr << "Pattern file does not exist: " << filename << std::endl;
        return {};
    }

    // Check it's a regular file
    if (!fs::is_regular_file(filename)) {
        std::cerr << "Path is not a regular file: " << filename << std::endl;
        return {};
    }

    // Check file size
    auto fileSize = fs::file_size(filename);
    if (fileSize == 0) {
        std::cerr << "Pattern file is empty: " << filename << std::endl;
        return {};
    }

    std::ifstream pattern(filename);
    if (pattern) {
        PatternArray grid;
        std::string line;
        auto gridW{0};
        auto gridH{0};
        while (std::getline(pattern, line)) {
            if (line[0] == RLE_DELIM) {
                continue;
            }

            if (line[0] == RLE_HEADER) {
                std::vector<char> discard;
                std::sscanf(
                        line.c_str(), "x = %d, y = %d, rule = %s", &gridW, &gridH, discard.data());
                break;
            }
        }

        std::string commands(std::istreambuf_iterator<char>{pattern}, {});
        if (!commands.empty()) {
            commands.erase(
                    std::remove_if(commands.begin(), commands.end(), ::isspace), commands.end());
        }

        std::stringstream ss;
        std::string cmdString;
        std::string blancLine(gridW, PTEXT_DEAD);
        for (const auto& command : commands) {
            if (std::isdigit(command)) {
                ss << command;
            } else {
                int count = 1;
                if (ss.rdbuf()->in_avail() != 0) {
                    ss >> count;
                    ss.str({});
                    ss.clear();
                }

                switch (command) {
                case RLE_DEAD:
                    cmdString.append(count, PTEXT_DEAD);
                    break;
                case RLE_LIVE:
                    cmdString.append(count, PTEXT_LIVE);
                    break;
                case RLE_EOL:
                    if (cmdString.length() < gridW) {
                        cmdString.append(gridW - cmdString.length(), PTEXT_DEAD);
                    }
                    grid.push_back(cmdString);
                    cmdString = {};

                    if (count > 1) {
                        for (auto i = 0; i < count - 1; ++i) {
                            grid.push_back(blancLine);
                        }
                    }
                    break;
                case RLE_EOD:
                    if (cmdString.length() < gridW) {
                        cmdString.append(gridW - cmdString.length(), PTEXT_DEAD);
                    }
                    grid.push_back(cmdString);

                    if (grid.size() != gridH) {
                        for (auto i = 0; i < gridH - grid.size(); ++i) {
                            grid.push_back(blancLine);
                        }
                    }
                    return grid;
                }
            }
        }
    }

    return {};
}
}  // namespace gol