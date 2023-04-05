/// \file GOLConfig.h
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
#include "ConwayDefs.h"

#include <nlohmann/json.hpp>

#include <cstdint>
#include <iosfwd>
#include <unordered_map>

using json = nlohmann::json;

static constexpr char GOL_CONFIG_NAME[]{"gol_config.json"};

static constexpr std::uint32_t COLOR_ASLEEP{0X003F5CFF};
static constexpr std::uint32_t COLOR_LONELY{0X58508DFF};
static constexpr std::uint32_t COLOR_CHOKED{0XBC5090FF};
static constexpr std::uint32_t COLOR_LIVING{0XFFA600FF};
static constexpr std::uint32_t COLOR_REBORN{0XFF6361FF};

namespace gol {
///
class GOLConfig {
public:
    GOLConfig();
    ~GOLConfig() = default;

    std::uint32_t getCellColor(CellPending cellPending) {
        return m_cellColors[cellPending];
    }

private:
    std::uint32_t convertStateColor(const char* jsonKey);
    void readStateColors();

    json m_json{};

    std::uint32_t m_colorAsleep{COLOR_ASLEEP};
    std::uint32_t m_colorLonely{COLOR_LONELY};
    std::uint32_t m_colorChoked{COLOR_CHOKED};
    std::uint32_t m_colorLiving{COLOR_LIVING};
    std::uint32_t m_colorReborn{COLOR_REBORN};

    std::unordered_map<CellPending, std::uint32_t> m_cellColors{
            {CELL_ASLEEP, m_colorAsleep},
            {CELL_LONELY, m_colorLonely},
            {CELL_CHOKED, m_colorChoked},
            {CELL_LIVING, m_colorLiving},
            {CELL_REBORN, m_colorReborn}};
};
}  // namespace gol