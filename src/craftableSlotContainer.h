#pragma once
#include "inventory.h"
struct craftableSlotContainer :inventory
{
	uiSlotContainer* outputSlot = nullptr;

	itemStack currentRecipeResult = itemStack();

	//can be multiple containers, like for the anvil
	std::vector<uiSlotContainer*> inputContainers = std::vector<uiSlotContainer*>();

	virtual itemStack calculateOutput() = 0;
	virtual void substractCraftingIngredients();
	void recalculateOutputSlot();
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
};