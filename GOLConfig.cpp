/// \file GOLConfig.cpp
/// \author Norm Evangelista
/// \copyright (c) 2025 Norm Evangelista
// Copyright 2025 gnawme (Norm Evangelista)
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
#include "ConwayDefs.h"

#include <cstdint>
#include <filesystem>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace gol {
///
GOLConfig::GOLConfig() {
    namespace fs = std::filesystem;

    fs::path configPath(GOL_CONFIG_NAME);

    // Check if file exists first
    if (!fs::exists(configPath)) {
        std::clog << "Config file not found: " << configPath << ", using fallbacks" << std::endl;
        return;
    }

    std::clog << "Loading config from: " << fs::absolute(configPath) << std::endl;

    std::ifstream golConfig(configPath);
    if (!golConfig) {
        std::clog << "Unable to open config file, using fallbacks" << std::endl;
        return;
    }

    m_json = json::parse(golConfig);
    readStateColors();
    readDisplayParams();
    readLifeTick();
    readStartupDelay();
}

/// \note PRIVATE
void GOLConfig::computeScreenTiling() {
    m_screenTiling = std::make_pair(
            static_cast<unsigned int>(m_screenSize.first / m_tileSize),
            static_cast<unsigned int>(m_screenSize.second / m_tileSize));
    std::clog << "Set screen tiling to " << m_screenTiling.first << "x" << m_screenTiling.second
              << std::endl;
}

/// \note PRIVATE
std::uint32_t GOLConfig::convertDisplayParam(const char* jsonKey) {
    auto inValue = m_json["displayParams"][jsonKey];
    std::stringstream ss;
    ss << inValue.get<std::string>();

    std::uint32_t param{0};
    ss >> param;
    return param;
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
float GOLConfig::convertTileSize(const char* jsonKey) {
    auto inValue = m_json["displayParams"][jsonKey];
    std::stringstream ss;
    ss << inValue.get<std::string>();

    float tileSize{0.0};
    ss >> tileSize;
    return tileSize;
}

/// \note PRIVATE
void GOLConfig::readDisplayParams() {
    m_screenSize = std::make_pair(
            convertDisplayParam("widthInPixels"), convertDisplayParam("heightInPixels"));

    m_tileSize = convertTileSize("tileSize");
    std::clog << "Screen size " << m_screenSize.first << "x" << m_screenSize.second << std::endl;
    std::clog << "Tile size " << m_tileSize << std::endl;
    computeScreenTiling();
}

/// \note PRIVATE
void GOLConfig::readLifeTick() {
    auto inValue = m_json["lifeTickInSecs"];
    std::stringstream ss;
    ss << inValue.get<std::string>();

    ss >> m_lifeTick;
    std::clog << "Life tick " << m_lifeTick << std::endl;
}

/// \note PRIVATE
void GOLConfig::readStartupDelay() {
    auto inValue = m_json["startupDelayInSecs"];
    std::stringstream ss;
    ss << inValue.get<std::string>();

    ss >> m_startupDelay;
    std::clog << "Startup delay " << m_startupDelay << std::endl;
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
