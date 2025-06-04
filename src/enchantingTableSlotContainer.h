#pragma once
#include "inventory.h"
struct blockContainer;
struct enchantingTableSlotContainer :inventory
{
	uiSlotContainer* enchantmentSlot = nullptr;
	uiSlotContainer* lapisSlot = nullptr;
	enchantingTableSlotContainer();
	virtual bool addToEqualStacks(itemStack& s, itemStack*& emptySlot) override;
	virtual ~enchantingTableSlotContainer() override;
	virtual void mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider) override;
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData) override;
	bool hasEnchantableItem() const;
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s) override;
	ull enchantmentSeed = 0;
	//the enchantment options, from bottom to top
	std::vector<enchantment> enchantmentOptions[enchantmentOptionCount]{};
	int baseEnchantmentLevels[enchantmentOptionCount]
	{ 0,0,0 };
	void recalculateEnchantments();
	bool isAvailable(cint& optionIndex) const;
	//count bookshelves
	static int countBookShelves(blockContainer* containerIn, cveci2& position);
};