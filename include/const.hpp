#pragma once

#include <string>
#include "colorfile.hpp"
#include "configfile.hpp"

static constexpr float MS_TO_S = 0.001F;
static constexpr float S_TO_MS = 1000.0F;

namespace PATH {
    const std::string COLORS_FILE = "resources/data/colors";
    const std::string LAYOUT_FILE = "resources/data/layout";
    const std::string PARAMS_FILE = "resources/data/params";
    const std::string TIMING_FILE = "resources/data/timing";
    const std::string TUNING_FILE = "resources/data/tuning";
    const std::string FONT_FILE =   "resources/fonts/COURBD.TTF";
}

static ConfigFile<unsigned int> LAYOUT(PATH::LAYOUT_FILE);
static ConfigFile<float> PARAMS(PATH::PARAMS_FILE);
static ConfigFile<float> TIMING(PATH::TIMING_FILE);
static ConfigFile<double> TUNING(PATH::TUNING_FILE);
static ColorFile COLORS = ColorFile(PATH::COLORS_FILE);