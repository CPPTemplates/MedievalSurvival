#include "nbt/nbtSerializable.h"
#include "serverData.h"
#include "globalFunctions.h"
#pragma once
struct serverDataList: nbtSerializable {
	std::vector<serverData> servers = std::vector<serverData>();

	virtual void serializeMembers(nbtSerializer& s) override;
	bool serialize(cbool& write);

};