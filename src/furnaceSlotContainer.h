#pragma once
#include "inventory.h"
struct furnaceSlotContainer : inventory
{
	furnaceData* selectedFurnaceData = nullptr;
	uiSlotContainer* furnaceOutputSlot = nullptr;
	uiSlotContainer* furnaceInputSlot = nullptr;
	uiSlotContainer* furnaceFuelSlot = nullptr;
	resolutionTexture* burnProgressTexture = nullptr;
	resolutionTexture* litProgressTexture = nullptr;
	furnaceSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData) override;
	virtual ~furnaceSlotContainer() override;
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	virtual bool addStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	void updateRecipe();
};