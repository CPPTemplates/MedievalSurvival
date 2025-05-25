#pragma once
#include "craftingGridSlotContainer.h"
struct craftingTableSlotContainer : craftingGridSlotContainer
{
	craftingTableSlotContainer();
	bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~craftingTableSlotContainer() override;
};