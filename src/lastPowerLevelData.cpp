#include "lastPowerLevelData.h"
#include "tickableBlockContainer.h"
#include "nbt/nbtSerializer.h"
bool lastPowerLevelData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	lastPowerLevel = containerIn->getPowerLevel(position);
	return false;
}

void lastPowerLevelData::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"last power level"), (int&)lastPowerLevel);
}