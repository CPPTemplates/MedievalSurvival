#include "stdafx.h"
#include "tradingSlotContainerUI.h"
#include "folderList.h"
#include <resourcePack.h>
#include "human.h"
#include "dimension.h"
#include "Villager.h"
#include "VillagerProfessionData.h"
#include "VillagerLevelData.h"
#include "MinecraftJSONReader.h"
#include <resourceLoader.h>
#include <TextureLoader.h>
#include "Linkable.h"

stdPath tradingTexturesFolder = containerSpritesFolder / L"villager";
constexpr rectanglei2 firstTradeSlotRect = rectanglei2(5, 128, 88, 20);
constexpr rectanglei2 tradeSlotsRect = rectanglei2(5, 8, 88, 140);

TradingSlotContainerUI::TradingSlotContainerUI() :
	craftableSlotContainer()
{
	outputSlot = new uiSlotContainer(veci2(220, 112), new rectangularSlotContainer(veci2(1)));
	uiTexturePartToDraw = rectanglei2(0, 90, 276, 166);
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / L"villager.png");
	inputContainers = {
		new uiSlotContainer(veci2(136, 113), new rectangularSlotContainer(veci2(1))),
		new uiSlotContainer(veci2(162, 113), new rectangularSlotContainer(veci2(1)))
	};
	//hotbar and inventory will be linked up
	containers = { inputContainers[0], inputContainers[1], hotbarSlots, inventorySlots, outputSlot };
	inventorySlots->containerPixelPosition = veci2(108, 30);
	hotbarSlots->containerPixelPosition = veci2(108, 8);
}

void TradingSlotContainerUI::linkUp(inventoryForm* form)
{
	inventory::linkUp(form);

	recalculateOutputSlot();

}

Villager* TradingSlotContainerUI::getVillager()
{
	return (Villager*)getLinkable(selectedVillagerUUID);
}

//won't work for textures bigger than the texture size!
void renderNineSliceTexture(const resolutionTexture& nineSliceTexture, crectanglei2& rect, cmat3x3& transform, const texture& renderTarget) {
	//the texture uses nine slice scaling, which means that the rectangle is separated into nine slices
	cint& nineSliceBorder = 3;
	constexpr veci2 buttonSize = veci2(200, 20);
	for (veci2 slicePos : rectanglei2(veci2(), veci2(3))) {
		rectanglei2 slicedSourceRect = rectanglei2(buttonSize);
		rectanglei2 slicedDestRect = rect;
		for (int axis = 0; axis < 2; axis++) {
			switch (slicePos[axis])
			{
			case 0:
				slicedDestRect.size[axis] = nineSliceBorder;
				break;
			case 1:
				slicedDestRect.pos0[axis] += nineSliceBorder;
				slicedDestRect.size[axis] = rect.size[axis] - nineSliceBorder * 2;

				slicedSourceRect.pos0[axis] = nineSliceBorder;
				break;
			default:
				slicedDestRect.pos0[axis] += rect.size[axis] - nineSliceBorder;
				slicedDestRect.size[axis] = nineSliceBorder;

				slicedSourceRect.pos0[axis] = buttonSize[axis] - nineSliceBorder;
				break;
			}
		}
		slicedSourceRect.size = slicedDestRect.size;

		fillTransparentRectangle(slicedSourceRect, slicedDestRect, transform, nineSliceTexture, renderTarget);

	}
}

void TradingSlotContainerUI::drawExtraData(cmat3x3& transform, const gameRenderData& targetData)
{

	if (Villager* v = getVillager()) {
		rectanglei2 tradeSlotRect = firstTradeSlotRect;
		//render trades
		for (const Trade& currentTrade : v->trades) {

			renderNineSliceTexture(globalLoader[widgetTextureFolder / ((&currentTrade == this->selectedTrade) ? "button_highlighted.png" : "button.png")], tradeSlotRect, transform, targetData.renderTarget);
			for (const itemStack& wanted : currentTrade.wanted) {
				wanted.render(transform.multRectMatrix(crectangle2(tradeSlotRect.pos0 + vec2(6, 3), vec2(14))), targetData);
			}
			currentTrade.given.render(transform.multRectMatrix(crectangle2(tradeSlotRect.pos0 + vec2(68, 3), vec2(14))), targetData);
			fillTransparentRectangle(rectangle2(0, 0, 10, 9), crectangle2(tradeSlotRect.pos0 + vec2(55, 6), vec2(10, 9)), transform, globalLoader[tradingTexturesFolder / (currentTrade.itemsInStock ? L"trade_arrow.png" : L"trade_arrow_out_of_stock.png")], targetData.renderTarget);
			tradeSlotRect.y -= tradeSlotRect.h;
		}
	}
}

itemStack TradingSlotContainerUI::calculateOutput()
{
	if (Villager* v = getVillager()) {
		const VillagerLevel& level = v->getLevel();
		constexpr int slotsPerLevel = 2;
		int requiredTradeSlots = ((int)level + 1) * slotsPerLevel;

		size_t slotIndex = v->trades.size();
		if (slotIndex != requiredTradeSlots) {
			size_t slotLevel = slotIndex / slotsPerLevel;

			v->trades.resize(requiredTradeSlots);
			//fill up trade slots till the required amount
			for (; slotIndex < requiredTradeSlots; slotLevel++) {
				jsonContainer& tradesToChooseFrom = (*villagerProfessionDataList[v->profession]->trades)[L"slots"][villagerLevelDataList[slotLevel]->name][L"trades"];
				std::vector<int> choices = std::vector<int>();
				for (int relativeSlotIndex = 0; relativeSlotIndex < slotsPerLevel; relativeSlotIndex++, slotIndex++) {
					//each trade should be unique
					int tradeIndex;
					do {
						tradeIndex = randIndex(currentRandom, (int)tradesToChooseFrom.children.size());

					} while (arrayContains(choices, tradeIndex));
					jsonContainer& trade = tradesToChooseFrom.children[tradeIndex];
					Trade& currentTrade = v->trades[slotIndex];
					int currentWantedIndex = 0;
					for (const jsonContainer& wantedContainer : trade[L"wanted"].children) {
						itemStack& item = currentTrade.wanted[currentWantedIndex];
						if (!readItemStack(wantedContainer, item) && (bool)item.stackItemID) {
							int minimum, maximum;
							if (convertToInt(wantedContainer[L"count"][L"min"].children[0].value, minimum) && convertToInt(wantedContainer[L"count"][L"max"].children[0].value, maximum)) {
								item.count = rand(currentRandom, minimum, maximum);
							}
						}
					}
					readItemStack(trade[L"given"], currentTrade.given);
					convertToInt(trade[L"stock"].children[0].value, currentTrade.maxItemsInstock);
					currentTrade.itemsInStock = currentTrade.maxItemsInstock;
					convertToDouble(trade[L"multiplier"].children[0].value, currentTrade.multiplier);
					convertToInt(trade[L"xp"].children[0].value, currentTrade.villagerXP);
					choices.push_back(tradeIndex);
				}
			}
		}
		if (!selectedTrade || !compareTrade(*selectedTrade)) {
			selectedTrade = nullptr;
			for (Trade& t : v->trades) {
				if (compareTrade(t)) {
					selectedTrade = &t;
					break;
				}
			}
		}
	}
	return selectedTrade ? selectedTrade->given : itemStack();
}

void TradingSlotContainerUI::substractCraftingIngredients()
{
	if (selectedTrade) {
		selectedTrade->itemsInStock--;
		for (int i = 0; i < 2; i++) {
			inputContainers[i]->getSlot(veci2())->add(-selectedTrade->wanted[i].count);
		}
		if (Villager* v = getVillager()) {
			v->tradingExperience += selectedTrade->villagerXP;
			linkedPlayer->addExperience(rand(currentRandom, 3, 6));
		}
	}
}

bool TradingSlotContainerUI::compareTrade(const Trade& trade)
{
	if (trade.itemsInStock) {
		for (int i = 0; i < 2; i++) {
			const itemStack& inputStack = *inputContainers[i]->getSlot(veci2());
			if (!trade.wanted[i].compare(inputStack) || trade.wanted[i].count > inputStack.count) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void TradingSlotContainerUI::mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider)
{
	if (Villager* v = getVillager()) {
		if (tradeSlotsRect.contains(pixelPosition)) {
			//-1 for correct rounding (70 - 70) / 20 = 0 but it should be -1
			cint& tradeIndex = (tradeSlotsRect.pos1().y - pixelPosition.y - 1) / firstTradeSlotRect.h;
			if (tradeIndex >= 0 && tradeIndex < v->trades.size()) {
				selectedTrade = &v->trades[tradeIndex];
				//try to fill in stacks from inventory

			}
			return;
		}
	}
	craftableSlotContainer::mouseDown(pixelPosition, button, divider);
}
