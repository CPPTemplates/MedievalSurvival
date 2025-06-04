#include "VillagerProfessionData.h"
#include "include/filesystem/jsonReader.h"
idList<VillagerProfessionData*, ProfessionID> villagerProfessionDataList = idList<VillagerProfessionData*, ProfessionID>({
	new VillagerProfessionData(L"unemployed"),
	new VillagerProfessionData(L"nitwit"),
	new VillagerProfessionData(L"armorer"),
	new VillagerProfessionData(L"butcher"),
	new VillagerProfessionData(L"cartographer"),
	new VillagerProfessionData(L"cleric"),
	new VillagerProfessionData(L"farmer"),
	new VillagerProfessionData(L"fisherman"),
	new VillagerProfessionData(L"fletcher"),
	new VillagerProfessionData(L"leatherworker"),
	new VillagerProfessionData(L"librarian"),
	new VillagerProfessionData(L"mason"),
	new VillagerProfessionData(L"shepherd"),
	new VillagerProfessionData(L"toolsmith"),
	new VillagerProfessionData(L"weaponsmith"),
	});

VillagerProfessionData::~VillagerProfessionData()
{
	delete trades;
}
