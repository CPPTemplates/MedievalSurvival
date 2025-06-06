#pragma once
#include "craftableSlotContainer.h"
struct craftingGridSlotContainer : craftableSlotContainer
{
	uiSlotContainer* craftingSlots = nullptr;
	virtual itemStack calculateOutput() override;
	craftingGridSlotContainer(uiSlotContainer* craftingSlots);
};