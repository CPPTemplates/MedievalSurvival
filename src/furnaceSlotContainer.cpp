#include "entity.h"
#include "furnaceSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "furnaceData.h"
#include "human.h"
#include "furnaceRecipe.h"
#include "dimension.h"
#include "stackDivider.h"
#include <resourceLoader.h>
#include <itemData.h>
#include <TextureLoader.h>

furnaceSlotContainer::furnaceSlotContainer() {
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / L"furnace.png");
	containers.push_back(furnaceFuelSlot = new uiSlotContainer(cveci2(56, 97), nullptr));
	containers.push_back(furnaceInputSlot = new uiSlotContainer(cveci2(56, 133), nullptr));
	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);
	containers.push_back(furnaceOutputSlot = new uiSlotContainer(cveci2(116, 116), nullptr));
}

bool furnaceSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot) {
	return furnaceInputSlot->addToEqualStacks(s, emptySlot) || furnaceFuelSlot->addToEqualStacks(s, emptySlot);
}

void furnaceSlotContainer::drawExtraData(cmat3x3& transform, const gameRenderData& targetData) {
	//draw fire and progress bar
	//draw fire
	if (selectedFurnaceData->ticksFuelBurned) {
		fp burnProgress =
			selectedFurnaceData->ticksFuelBurned / (fp)selectedFurnaceData->ticksFuelToBurn;
		inventory::drawExtraData(globalLoader[containerSpritesFolder / L"furnace" / L"lit_progress.png"], crectanglei2(0, 0, 14, (int)(14 * (1.0 - burnProgress))), veci2(56, 116), transform, targetData);
	}
	//draw progress bar
	if (selectedFurnaceData->ticksCooked) {
		fp cookProgress = selectedFurnaceData->ticksCooked /
			(fp)selectedFurnaceData->currentRecipe->cookingTime;
		inventory::drawExtraData(globalLoader[containerSpritesFolder / L"furnace" / L"burn_progress.png"], crectanglei2(0, 0, (int)(24 * cookProgress), 16),
			veci2(79, 115), transform, targetData);
	}
}

void furnaceSlotContainer::clickedOnItem(cmb& button, stackDivider& divider,
	uiSlotContainer* selectedSlotContainer,
	veci2 selectedSlot) {
	human* currentHuman = (human*)linkedPlayer;
	if (selectedSlotContainer == furnaceOutputSlot) {
		itemStack* s = selectedSlotContainer->getSlot(selectedSlot);
		if (divider.addStack(*s)) {
			//got all items
			currentHuman->addExperience(
				roundRandom(currentRandom, selectedFurnaceData->collectedExperience));
			selectedFurnaceData->collectedExperience = 0;
		}
	}
	else {
		inventory::clickedOnItem(button, divider, selectedSlotContainer, selectedSlot);
		if (selectedSlotContainer == furnaceInputSlot) {
			updateRecipe();
		}
	}
	currentHuman->selectedContainerContainer->addUpdatePosition(
		currentHuman->selectedContainerPosition);
}

bool furnaceSlotContainer::canAddStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	return containerToAddTo != furnaceFuelSlot || itemList[s.stackItemID]->fuelTicks;
}

bool furnaceSlotContainer::addStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	if (inventory::addStack(containerToAddTo, s)) {
		updateRecipe();
		return true;
	}
	return  false;
}

void furnaceSlotContainer::updateRecipe()
{
	furnaceRecipe* newRecipe = findRecipe(furnaceRecipes,
		furnaceInputSlot->linkedContainer);  // getFurnaceRecipe(furnaceInputSlot->linkedContainer);
	if (selectedFurnaceData->currentRecipe != newRecipe) {
		selectedFurnaceData->currentRecipe = newRecipe;
		selectedFurnaceData->ticksCooked = 0;
	}
}

furnaceSlotContainer::~furnaceSlotContainer() {
	delete furnaceInputSlot;
	delete furnaceOutputSlot;
	delete furnaceFuelSlot;
	delete hotbarSlots;
	delete inventorySlots;
}