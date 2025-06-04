#include "inventory.h"
#pragma once
struct TradingSlotContainerUI :public inventory {
	uiSlotContainer* emeraldSlot;
	uiSlotContainer* tradingInputSlot;
	uiSlotContainer* tradingOutputSlot;
	uuid selectedVillagerUUID = uuid();
	TradingSlotContainerUI();
	virtual void linkUp(struct inventoryForm* form) override;
	struct Villager* getVillager();
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData) override;
};