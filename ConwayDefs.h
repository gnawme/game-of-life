/// \file ConwayDefs.h
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
#pragma once

#include <string>
#include <vector>

namespace gol {
class ConwayCell;
using CellRow = std::vector<ConwayCell>;
using CellArray = std::vector<CellRow>;

using PatternArray = std::vector<std::string>;
using ScreenSize = std::pair<unsigned int, unsigned int>;

static constexpr ScreenSize GOL_SCREEN_720P{1280, 720};
static constexpr float GOL_TILE_SIZE{16.0};
static constexpr ScreenSize GOL_TILING_720P{
        static_cast<unsigned int>(GOL_SCREEN_720P.first / GOL_TILE_SIZE),
        static_cast<unsigned int>(GOL_SCREEN_720P.second / GOL_TILE_SIZE)};
}  // namespace gol