/// \file GOLFile.h
#pragma once
#include "ConwayDefs.h"

namespace gol {

constexpr char PTEXT_LIVE{'O'};
constexpr char PTEXT_DEAD{'.'};

PatternArray readPlaintextPatternFile(const char* filename);

PatternArray readRLEPatternFile(const char* filename);
}  // namespace gol