#include "craftableSlotContainer.h"
#pragma once
struct TradingSlotContainerUI :public craftableSlotContainer {
	uuid selectedVillagerUUID = uuid();
	struct Trade* selectedTrade = nullptr;
	TradingSlotContainerUI();
	virtual void linkUp(struct inventoryForm* form) override;
	struct Villager* getVillager();
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData) override;
	virtual itemStack calculateOutput() override;
	virtual void substractCraftingIngredients() override;
	bool compareTrade(const Trade& trade);
	virtual void mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider) override;
};