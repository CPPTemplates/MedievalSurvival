#pragma once
#include "inventory.h"
#include "brewingStandData.h"
struct brewingStandSlotContainer :inventory
{
	brewingstandData* selectedBrewingStandData = nullptr;
	uiSlotContainer* blazePowderSlot = nullptr;
	uiSlotContainer* ingredientSlot = nullptr;
	uiSlotContainer* potionSlots[brewingStandPotionCapacity]{};
	brewingStandSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData) override;
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
	virtual ~brewingStandSlotContainer() override;
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
};