/// \file GOLFile.h
#pragma once
#include "ConwayDefs.h"

#include <string>

namespace gol {

constexpr char PTEXT_LIVE{'O'};
constexpr char PTEXT_DEAD{'.'};

constexpr char CELLS[]{"cells"};
constexpr char RLE[]{"rle"};

class GOLFile {
public:
    GOLFile() = default;
    GOLFile(const char* filename);
    GOLFile(const std::string& filename);

    [[nodiscard]] std::string getFilename() const;
    PatternArray getPatternArray();

    PatternArray readPatternFile(const char* filename);
    PatternArray readPatternFile(const std::string& filename);

private:
    std::string getExtension(const std::string& filename);
    PatternArray readPlaintextPatternFile(const std::string& filename);
    PatternArray readRLEPatternFile(const std::string& filename);

    std::string m_filename{};
    std::string m_extension{CELLS};
};

}  // namespace gol