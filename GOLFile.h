/// \file GOLFile.h
/// \author Norm Evangelista
/// \copyright (c) 2023 Norm Evangelista
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
#pragma once
#include "ConwayDefs.h"

#include <string>

namespace gol {

constexpr char CELLS[]{"cells"};
constexpr char PTEXT_LIVE{'O'};
constexpr char PTEXT_DEAD{'.'};
constexpr char PTEXT_DELIM{'!'};

constexpr char RLE[]{"rle"};
constexpr char RLE_LIVE{'o'};
constexpr char RLE_DEAD{'b'};
constexpr char RLE_DELIM{'#'};
constexpr char RLE_HEADER{'x'};
constexpr char RLE_EOL{'$'};
constexpr char RLE_EOD{'!'};

class GOLFile {
public:
    GOLFile() = default;
    GOLFile(const char* filename);
    GOLFile(const std::string& filename);

    std::string getFilename() const;
    PatternArray getPatternArray();

    PatternArray readPatternFile(const char* filename);
    PatternArray readPatternFile(const std::string& filename);

private:
    std::size_t prescanPlaintext(std::ifstream& pattern);
    std::string getExtension(const std::string& filename);
    PatternArray readPlaintextPatternFile(const std::string& filename);
    PatternArray readRLEPatternFile(const std::string& filename);

    std::string m_filename{};
    std::string m_extension{CELLS};
};

}  // namespace gol