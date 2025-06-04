#include "VillagerLevel.h"
#include <ranges>
#include <type/EnumTricks.h>
#include <VillagerLevelData.h>

VillagerLevel getVillagerLevel(cint& tradingExperience)
{
	for (VillagerLevel level : EnumArray<VillagerLevel>()) {
		if (villagerLevelDataList[level]->xpNeeded > tradingExperience)
			return (VillagerLevel)(level - 1);
	}
	return VillagerLevel::Master;
}
