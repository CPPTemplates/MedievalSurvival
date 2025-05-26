#include "brewingStandSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "human.h"
#include "dimension.h"
#include "stackDivider.h"
#include <resourceLoader.h>
brewingStandSlotContainer::brewingStandSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / std::wstring(L"brewing_stand.png"));
	containers.push_back(blazePowderSlot = new uiSlotContainer(cveci2(17, 133), nullptr));
	containers.push_back(ingredientSlot = new uiSlotContainer(cveci2(79, 133), nullptr));
	containers.push_back(potionSlots[0] = new uiSlotContainer(cveci2(56, 99), nullptr));
	containers.push_back(potionSlots[1] = new uiSlotContainer(cveci2(79, 92), nullptr));
	containers.push_back(potionSlots[2] = new uiSlotContainer(cveci2(102, 99), nullptr));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
}

bool brewingStandSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot)
{
	if (s.count == 0)
	{
		return false;
	}
	if (s.stackItemID == itemID::blaze_powder)
	{
		return blazePowderSlot->addToEqualStacks(s, emptySlot);
	}
	else if (s.stackItemID == itemID::potion)
	{
		for (int i = 0; i < brewingStandPotionCapacity; i++)
		{
			if (potionSlots[i]->addToEqualStacks(s, emptySlot))
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return ingredientSlot->addToEqualStacks(s, emptySlot);
	}
}

void brewingStandSlotContainer::drawExtraData(cmat3x3& transform, const texture& renderTarget)
{
	//draw fire and progress bar
	//draw fire
	if (selectedBrewingStandData->ticksBrewed)
	{
		cfp& brewProgress = selectedBrewingStandData->ticksBrewed / (fp)brewingBatchTime;
		cint& brewTextureHeight = 27;
		cint& brewTexturePart = (int)(brewTextureHeight * brewProgress);
		inventory::drawExtraData(globalLoader[containerSpritesFolder / L"brewing_stand" / L"brew_progress.png"], crectanglei2(0, brewTextureHeight - brewTexturePart, 33, brewTexturePart), veci2(97, 121 + (brewTextureHeight - brewTexturePart)), transform, renderTarget);
	}
	//draw progress bar
	if (selectedBrewingStandData->fuelLeft)
	{
		cfp& partLeft = selectedBrewingStandData->fuelLeft / (fp)blazePowderBrewingBatchCount;
		inventory::drawExtraData(globalLoader[containerSpritesFolder / L"brewing_stand" / L"fuel_length.png"], crectanglei2(0, 0, (int)(18 * partLeft), 4), veci2(60, 118), transform, renderTarget);
	}
}

void brewingStandSlotContainer::clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot)
{
	//reset process when adding bottle

	inventory::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);

	human* currentHuman = (human*)linkedPlayer;
	currentHuman->selectedContainerContainer->addUpdatePosition(currentHuman->selectedContainerPosition);
}

brewingStandSlotContainer::~brewingStandSlotContainer()
{
	delete selectedBrewingStandData;
	delete blazePowderSlot;
	for (int i = 0; i < brewingStandPotionCapacity; i++)
	{
		delete potionSlots[i];
	}
	delete hotbarSlots;
	delete inventorySlots;
}

bool brewingStandSlotContainer::canAddStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	if (containerToAddTo == blazePowderSlot)
	{
		return s.stackItemID == itemID::blaze_powder;
	}
	else
	{
		return inventory::canAddStack(containerToAddTo, s);
	}
}