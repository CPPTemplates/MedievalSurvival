#include "dispenserSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
dispenserSlotContainer::dispenserSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"dispenser.png"));
	containers.push_back(dispenserSlots = new uiSlotContainer(cveci2(62, 97), nullptr));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool dispenserSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot)
{
	return dispenserSlots->addToEqualStacks(s, emptySlot);
}

dispenserSlotContainer::~dispenserSlotContainer()
{
	delete dispenserSlots;
	delete hotbarSlots;
	delete inventorySlots;
}