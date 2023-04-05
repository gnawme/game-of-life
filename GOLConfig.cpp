/// \file GOLConfig.cpp
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
#include "GOLConfig.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

namespace gol {
///
GOLConfig::GOLConfig() {
    std::ifstream golConfig(GOL_CONFIG_NAME);
    if (!golConfig) {
        std::clog << "Unable to open config file " << GOL_CONFIG_NAME << ", using fallbacks"
                  << std::endl;
        return;
    }

    m_json = json::parse(golConfig);
    std::clog << m_json.dump(4) << std::endl;
    readStateColors();
}

/// \note PRIVATE
std::uint32_t GOLConfig::convertStateColor(const char* jsonKey) {
    auto inValue = m_json["stateColors"][jsonKey];
    std::stringstream ss;
    ss << std::hex << inValue.get<std::string>();

    std::uint32_t color{0};
    ss >> color;
    return color;
}

/// \note PRIVATE
void GOLConfig::readStateColors() {
    m_colorAsleep = convertStateColor("colorAsleep");
    m_cellColors[CELL_ASLEEP] = m_colorAsleep;

    m_colorLonely = convertStateColor("colorLonely");
    m_cellColors[CELL_LONELY] = m_colorLonely;

    m_colorChoked = convertStateColor("colorChoked");
    m_cellColors[CELL_LONELY] = m_colorChoked;

    m_colorLiving = convertStateColor("colorLiving");
    m_cellColors[CELL_LIVING] = m_colorLiving;

    m_colorReborn = convertStateColor("colorReborn");
    m_cellColors[CELL_REBORN] = m_colorReborn;
}

}  // namespace gol
