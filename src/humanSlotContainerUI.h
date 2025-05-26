#pragma once
#include "craftingGridSlotContainer.h"
struct humanSlotContainerUI :craftingGridSlotContainer
{
	uiSlotContainer* armorSlots;
	uiSlotContainer* leftHandSlot;
	humanSlotContainerUI();
	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot) override;
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, cveci2& position, itemStack& s) override;
	virtual bool addStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	void playEquipSound(itemStack& s);
};