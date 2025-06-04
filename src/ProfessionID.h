#include "itemID.h"
#pragma once
enum ProfessionID {
	Unemployed,
	Nitwit,
	Armorer,
	Butcher,
	Cartographer,
	Cleric,
	Farmer,
	Fisherman,
	Fletcher,
	Leatherworker,
	Librarian,
	Mason,
	Shepherd,
	Toolsmith,
	Weaponsmith,
	size
};

constexpr bool isEmployed(const ProfessionID& profession) {
	return (int)profession > (int)Nitwit;
}

constexpr ProfessionID getProfession(const blockID& jobSiteBlock) {
	switch (jobSiteBlock)
	{
	case blockID::blast_furnace:
		return ProfessionID::Armorer;
	case blockID::smoker:
		return ProfessionID::Butcher;
	case blockID::barrel:
		return ProfessionID::Fisherman;
	case blockID::fletchingtable:
		return ProfessionID::Fletcher;
	case blockID::smithing_table:
		return ProfessionID::Toolsmith;
	case blockID::grindstone:
		return ProfessionID::Weaponsmith;
	case blockID::composter:
		return ProfessionID::Farmer;
	case blockID::lectern:
		return ProfessionID::Librarian;
	case blockID::loom:
		return ProfessionID::Shepherd;
	case blockID::cartography_table:
		return ProfessionID::Cartographer;
	case blockID::brewing_stand:
		return ProfessionID::Cleric;
	case blockID::cauldron:
		return ProfessionID::Leatherworker;
		//missing stonecutter
	default:
		return ProfessionID::Unemployed;
	}
}

constexpr bool isJobBlock(const blockID& block) {
	return getProfession(block);
}