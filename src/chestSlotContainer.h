#pragma once
#include "inventory.h"
struct chestSlotContainer :inventory
{
	chestData* selectedChestData = nullptr;
	uiSlotContainer* chestSlots = nullptr;
	chestSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~chestSlotContainer() override;
};