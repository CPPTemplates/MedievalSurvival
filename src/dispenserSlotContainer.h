#pragma once
#include "inventory.h"
#include "dispenserData.h"
struct dispenserSlotContainer : inventory
{
	dispenserData* selectedDispenserData = nullptr;
	uiSlotContainer* dispenserSlots = nullptr;
	dispenserSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~dispenserSlotContainer() override;
};