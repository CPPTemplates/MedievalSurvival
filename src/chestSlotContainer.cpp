#include "chestSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
chestSlotContainer::chestSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"shulker_box.png"));
	containers.push_back(chestSlots = new uiSlotContainer(cveci2(0x8, 96), nullptr));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool chestSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot)
{
	return chestSlots->addToEqualStacks(s, emptySlot);
}

chestSlotContainer::~chestSlotContainer()
{
	delete chestSlots;
	delete hotbarSlots;
	delete inventorySlots;
}