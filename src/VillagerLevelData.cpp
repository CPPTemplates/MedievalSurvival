#include "VillagerLevelData.h"

idList<villagerLevelData*, VillagerLevel> villagerLevelDataList = idList<villagerLevelData*, VillagerLevel>({
	new villagerLevelData(L"novice", 0),
	new villagerLevelData(L"apprentice", 10),
	new villagerLevelData(L"journeyman", 70),
	new villagerLevelData(L"expert", 150),
	new villagerLevelData(L"master", 250)
	});