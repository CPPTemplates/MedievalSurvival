#include "Trades.h"
#include "filesystem/jsonReader.h"
#include "nbt/nbtSerializer.h"
#include <resourcePack.h>
#include <folderList.h>
#include <filesystem/textfile.h>
#include "ProfessionID.h"
#include "VillagerProfessionData.h"
#include <resourceLoader.h>

//the data keeps being saved in JSON and is only queried when needed. in c++, saving to custom structs only costs more coding.
void readTrades()
{
	for (const auto& currentFolder : getResourceLocations(msNameSpaceDataFolder / L"trade") | std::views::reverse)
		for (const auto& fileIterator : stdFileSystem::directory_iterator(currentFolder))
		{
			std::wstring path = fileIterator.path().wstring();
			jsonContainer* content = new jsonContainer(readJson(stringToWString(readAllText(path))));
			// this might cause double recipes, but the recipes which matter the most will be on top, so it'll match correctly
			// todo: implement algorithm that eliminates duplicate recipes to save memory
			ProfessionID profession = villagerProfessionDataList.getIDByName((*content)[std::wstring(L"profession")].children[0].value);
			if (profession != (ProfessionID)-1)
				villagerProfessionDataList[profession]->trades = content;
		}
}

void Trade::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, L"multiplier", multiplier);
	serializeNBTValue(s, L"wanted", wanted);
	serializeNBTValue(s, L"given", given);
	serializeNBTValue(s, L"max items in stock", maxItemsInstock);
	serializeNBTValue(s, L"stock", itemsInStock);
}
