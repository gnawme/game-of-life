/// \file GOLFile.h
#pragma once
#include "ConwayDefs.h"

namespace gol {

PatternArray readPlaintextPatternFile(const char* filename);

PatternArray readRLEPatternFile(const char* filename);
}  // namespace gol