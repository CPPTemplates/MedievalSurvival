#include "world.h"
#include "gameTime.h"
lightLevel getVisibleSunLightLevel(const lightLevel& realSunLightLevel)
{
	return math::minimum((lightLevel)timeToLightLevel.getValue(getTimeOfDay(currentWorld->currentTime)), realSunLightLevel);
}