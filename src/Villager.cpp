#include "stdafx.h"
#include "Villager.h"
#include "taskList.h"
#include "wanderAI.h"
#include "nbt/serializeList.h"
#include "nbt/serializeUUID.h"
#include "workAI.h"
constexpr rectanglei2 villagerHeadTextureRect = rectanglei2(0, 46, 8, 10);
constexpr rectanglei2 villagerLegTextureRect = rectanglei2(0, 26, 4, 12);
constexpr rectanglei2 villagerBodyTextureRect = rectanglei2(16, 26, 6, 12);
constexpr rectanglei2 villagerUpperArmTextureRect = rectanglei2(44, 30, 4, 8);
Villager::Villager() : humanoid(entityID::villager)
{
	tasks = new taskList(this, { 
		   new WorkAI(this),
		   new wanderAI(this)
		});
	initializeBodyParts(villagerHeadTextureRect, villagerBodyTextureRect, villagerLegTextureRect, villagerLegTextureRect, villagerUpperArmTextureRect, villagerUpperArmTextureRect, humanoidPixelSize);
	itemHolding = new itemStack();
	//1 in 10 that it'll be a nitwit
	profession = randChance(currentRandom, 10) ? ProfessionID::Nitwit : ProfessionID::Unemployed;
}

void Villager::serializeMembers(nbtSerializer& s)
{
	humanoid::serializeMembers(s);
	serializeNBTValue(s, L"trading experience", tradingExperience);
	serializeNBTValue(s, L"profession", profession);
	serializeNBTValue(s, L"trades", trades);
	serializeNBTValue(s, L"cured by", curedBy);
}

VillagerLevel Villager::getLevel() const
{
	return getVillagerLevel(tradingExperience);
}
