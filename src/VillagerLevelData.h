#pragma once
#include <interface/inamable.h>
#include "VillagerLevel.h"
#include "idList.h"

struct villagerLevelData : INamable {
	int xpNeeded;
	villagerLevelData(const std::wstring& name, cint& xpNeeded) : INamable(name), xpNeeded(xpNeeded){}
};
extern idList<villagerLevelData*, VillagerLevel> villagerLevelDataList;