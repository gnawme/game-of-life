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
#include <unordered_map>
#include <vector>

namespace gol {
class ConwayCell;
using CellRow = std::vector<ConwayCell>;
using CellArray = std::vector<CellRow>;

using PatternArray = std::vector<std::string>;
using ScreenSize = std::pair<unsigned int, unsigned int>;

enum CellPending { CELL_ASLEEP, CELL_LONELY, CELL_CHOKED, CELL_LIVING, CELL_REBORN };
static std::unordered_map<CellPending, std::string> PENDING_STATE{
        {CELL_ASLEEP, "CELL_ASLEEP"},
        {CELL_LONELY, "CELL_LONELY"},
        {CELL_CHOKED, "CELL_CHOKED"},
        {CELL_LIVING, "CELL_LIVING"},
        {CELL_REBORN, "CELL_REBORN"}};

}  // namespace gol