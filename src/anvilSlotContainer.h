#pragma once
#include "craftableSlotContainer.h"
struct anvilSlotContainer : craftableSlotContainer
{
	int itemsUsed = 0;

	anvilSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~anvilSlotContainer() override;
	virtual itemStack calculateRecipeResult() override;
	virtual void substractCraftingIngredients() override;
};