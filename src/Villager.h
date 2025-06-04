#include "humanoid.h"
#include "Trades.h"
#include "VillagerLevel.h"
#include "ProfessionID.h"
#pragma once
struct Villager : public humanoid {
	Villager();
	ProfessionID profession;
	std::vector<Trade> trades = std::vector<Trade>();
	//each player can cure a villager once
	std::vector<uuid> curedBy = std::vector<uuid>();

	int tradingExperience = 0;
	void serializeMembers(nbtSerializer& s) override;
	VillagerLevel getLevel() const;
};