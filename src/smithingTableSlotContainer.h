#pragma once
#include "craftableSlotContainer.h"
struct smithingTableSlotContainer : craftableSlotContainer
{
	uiSlotContainer* smithingInputSlot = nullptr;
	uiSlotContainer* netheriteIngotSlot = nullptr;
	smithingTableSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~smithingTableSlotContainer() override;
	virtual itemStack calculateRecipeResult() override;
	bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	void substractCraftingIngredients() override;
};