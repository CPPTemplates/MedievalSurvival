#include "smithingTableSlotContainer.h"
#include "resourcePack.h"
#include "folderList.h"
#include "idAnalysis.h"
#include "human.h"
#include "soundList.h"
#include "dimension.h"
#include "stackDivider.h"

smithingTableSlotContainer::smithingTableSlotContainer()
{
	uiTexture = loadTextureFromResourcePack(containerTextureFolder / L"smithing.png");
	containers.push_back(smithingInputSlot = new uiSlotContainer(cveci2(27, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(netheriteIngotSlot = new uiSlotContainer(cveci2(76, 103), new rectangularSlotContainer(cveci2(1))));
	containers.push_back(outputSlot = new uiSlotContainer(cveci2(134, 103), new rectangularSlotContainer(cveci2(1))));

	//hotbar and inventory will be linked up
	containers.push_back(hotbarSlots);
	containers.push_back(inventorySlots);

	inputContainers.push_back(smithingInputSlot);
	inputContainers.push_back(netheriteIngotSlot);
}

bool smithingTableSlotContainer::addToEqualStacks(itemStack& s, itemStack*& emptySlot)
{
	if (isMaterialArmor(s.stackItemID))
	{
		return smithingInputSlot->addToEqualStacks(s, emptySlot);
	}
	else if (s.stackItemID == itemID::netherite_ingot)
	{
		return netheriteIngotSlot->addToEqualStacks(s, emptySlot);
	}
	return false;
}

smithingTableSlotContainer::~smithingTableSlotContainer()
{
	delete inventorySlots;
	delete hotbarSlots;

	delete smithingInputSlot;

	delete netheriteIngotSlot;

	delete outputSlot;
}

itemStack smithingTableSlotContainer::calculateOutput()
{
	const itemStack& inputSlot = smithingInputSlot->linkedContainer->slots[0];

	itemStack resultingStack;

	if (netheriteIngotSlot->linkedContainer->slots[0].count && inputSlot.count)
	{
		resultingStack = inputSlot;
		if (isMaterialArmor(inputSlot.stackItemID))
		{
			resultingStack.stackItemID = getMaterialArmor(getMaterialArmorType(inputSlot.stackItemID), netheriteArmorTier);
		}
		else
		{
			resultingStack.stackItemID = getMaterialTool(getMaterialToolType(inputSlot.stackItemID), netheriteHarvestTier);
		}
	}
	else
	{
		resultingStack = itemStack();
	}

	return resultingStack;
}

bool smithingTableSlotContainer::canAddStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	if (containerToAddTo == smithingInputSlot)
	{
		return isMaterialArmor(s.stackItemID) || isMaterialTool(s.stackItemID);
	}
	else if (containerToAddTo == netheriteIngotSlot)
	{
		return s.stackItemID == itemID::netherite_ingot;
	}
	return true;
}

void smithingTableSlotContainer::substractCraftingIngredients()
{
	craftableSlotContainer::substractCraftingIngredients();
	human* currentHuman = (human*)linkedPlayer;
	smithingTableUseSound->playRandomSound(currentHuman->selectedContainerContainer, currentHuman->selectedContainerPosition + cvec2(0.5, 1));
}