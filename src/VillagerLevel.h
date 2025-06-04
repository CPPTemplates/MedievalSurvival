#pragma once
#include <globalFunctions.h>
enum VillagerLevel :int {
	Novice,
	Apprentice,
	Journeyman,
	Expert,
	Master,
	count
};

VillagerLevel getVillagerLevel(cint& tradingExperience);