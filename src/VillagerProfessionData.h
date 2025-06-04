#pragma once
#include <interface/inamable.h>
#include "ProfessionID.h"
#include "idList.h"
struct VillagerProfessionData : INamable {
	struct jsonContainer* trades = nullptr;
	VillagerProfessionData(const std::wstring& name) : INamable(name) {}
	~VillagerProfessionData();
};
extern idList<VillagerProfessionData*, ProfessionID> villagerProfessionDataList;