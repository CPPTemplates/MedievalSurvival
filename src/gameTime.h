#pragma once
#include "math/mathFunctions.h"
#include "constants.h"
namespace gameTime
{
    constexpr int day = 1000;
    constexpr int noon = 6000;
    constexpr int night = 13000;
    constexpr int midnight = 18000;
    constexpr int sunrise = 23000;
    constexpr int sunset = 12000;
}

constexpr fp getTimeOfDay(cfp& currentTime)
{
	return math::mod(currentTime, (fp)ticksPerDay);
}