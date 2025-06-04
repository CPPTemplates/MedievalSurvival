#pragma once
#include "slotContainer.h"
#include "interface/idestructable.h"
#include "rectangularSlotContainer.h"
struct uiSlotContainer :slotContainer, IDestructable
{
	veci2 containerPixelPosition;
	rectangularSlotContainer* linkedContainer;
	//containerpixelposition is relative to the rectangle of the container texture that is being rendered! the position is the bottom left corner of the slot. margins will be calculated automatically
	uiSlotContainer(veci2 containerPixelPosition, rectangularSlotContainer* linkedContainer = nullptr) :containerPixelPosition(containerPixelPosition), linkedContainer(linkedContainer) {}
	inline itemStack* getSlot(cveci2& pos) { return linkedContainer->getSlot(pos); }
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	//virtual void clickedOnItem(cmb& button, itemStack& stackHolding, veci2 selectedSlot);
	virtual ~uiSlotContainer() override;
};