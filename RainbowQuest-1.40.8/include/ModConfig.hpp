#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(RainbowConfig) {
    CONFIG_VALUE(RainbowEnabled, bool, "Rainbow Enabled", true);
    CONFIG_VALUE(RainbowSpeed, float, "Rainbow Speed", 0.5f);
};
