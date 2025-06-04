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

stdPath tradingTexturesFolder = containerSpritesFolder / L"villager";

TradingSlotContainerUI::TradingSlotContainerUI() : emeraldSlot(new uiSlotContainer(veci2(136, 113), new rectangularSlotContainer(veci2(1)))), tradingInputSlot(new uiSlotContainer(veci2(162, 113), new rectangularSlotContainer(veci2(1)))), tradingOutputSlot(new uiSlotContainer(veci2(220, 112), new rectangularSlotContainer(veci2(1)))), inventory()
{
	this->uiTexturePartToDraw = rectanglei2(0, 90, 276, 166);
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / L"villager.png");
	//hotbar and inventory will be linked up
	containers = { emeraldSlot, tradingInputSlot, hotbarSlots, inventorySlots, tradingOutputSlot };
	inventorySlots->containerPixelPosition = veci2(108, 30);
	hotbarSlots->containerPixelPosition = veci2(108, 8);
}

void TradingSlotContainerUI::linkUp(inventoryForm* form)
{
	inventory::linkUp(form);
	if (Villager* v = getVillager()) {
		const VillagerLevel& level = v->getLevel();
		constexpr int slotsPerLevel = 2;
		int requiredTradeSlots = ((int)level + 1) * slotsPerLevel;

		size_t slotIndex = v->trades.size();
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
				choices.push_back(tradeIndex);
			}
		}
	}
}

Villager* TradingSlotContainerUI::getVillager()
{
	return (Villager*)linkedPlayer->dimensionIn->findUUID(linkedPlayer->position, humanArmRange + mobSizeMargin, selectedVillagerUUID);
}

//won't work for buttons bigger than the texture size!
void renderButton(crectanglei2& rect, cmat3x3& transform, const texture& renderTarget) {
	//the button uses nine slice scaling, which means that the rectangle is separated into nine slices
	cint& nineSliceBorder = 3;
	const resolutionTexture& buttonTexture = globalLoader[widgetTextureFolder / "button.png"];
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

		fillTransparentRectangle(slicedSourceRect, slicedDestRect, transform, buttonTexture, renderTarget);

	}
}

void TradingSlotContainerUI::drawExtraData(cmat3x3& transform, const gameRenderData& targetData)
{

	if (Villager* v = getVillager()) {
		rectanglei2 tradeSlotRect = rectanglei2(5, 128, 88, 20);
		//render trades
		for (const Trade& currentTrade : v->trades) {

			renderButton(tradeSlotRect, transform, targetData.renderTarget);
			for (const itemStack& wanted : currentTrade.wanted) {
				wanted.render(transform.multRectMatrix(crectangle2(tradeSlotRect.pos0 + vec2(6, 3), vec2(14))), targetData);
			}
			currentTrade.given.render(transform.multRectMatrix(crectangle2(tradeSlotRect.pos0 + vec2(68, 3), vec2(14))), targetData);
			fillTransparentRectangle(rectangle2(0, 0, 10, 9), crectangle2(tradeSlotRect.pos0 + vec2(55, 6), vec2(10, 9)), transform, globalLoader[tradingTexturesFolder / (currentTrade.itemsInStock ? L"trade_arrow.png" : L"trade_arrow_out_of_stock.png")], targetData.renderTarget);
			tradeSlotRect.y -= tradeSlotRect.h;
		}
	}
}
