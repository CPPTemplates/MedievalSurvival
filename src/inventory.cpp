#include "inventory.h"
#include "human.h"
#include "stackDivider.h"
#include "gameControl.h"
#include "inventoryForm.h"

void inventory::clickedOnItem(cmb& button, stackDivider& divider,
	uiSlotContainer* selectedSlotContainer, veci2 selectedSlot)
{
	itemStack* occupyingSlot = selectedSlotContainer->linkedContainer->getSlot(
		selectedSlot);
	if (linkedPlayer->screen.mostRecentInput.holdingDownKey(vk::LShift) || linkedPlayer->screen.mostRecentInput.holdingDownKey(vk::RShift) && button == mb::Left) {
		for (uiSlotContainer* destContainer : containers) {
			if (destContainer != selectedSlotContainer && canAddStack(destContainer, *occupyingSlot)) {
				//move stack to other container
				addStack(destContainer, *occupyingSlot);
			}
		}
	}
	else {
		if (divider.originalStack.count == 0 || canAddStack(selectedSlotContainer, selectedSlot, divider.originalStack))
		{
			divider.interactWith(occupyingSlot, button);
		}
	}

	// if (divider.originalStack.count) {
	//     if (canAddStack(selectedSlotContainer, divider)) {
	//         if (!onAndroid && button == (mb) sf::Mouse::Right) {
	//             //selectedSlotContainer->linkedContainer->getSlot(selectedSlot)->addStack(
	//             //        divider, 1);
	//         } else {
	//             divider.interactWith(occupyingSlot);
	//             //if (occupyingSlot->stackItemID == divider.originalStack.stackItemID &&
	//             //    occupyingSlot->compare(divider.originalStack)) {
	//             //    occupyingSlot->addStack(stackHolding);
	//             //} else {
	//             //    if (occupyingSlot->count) {
	//             //        //can'T divide original stack when swapping items
	//             //        slotsDividingOver.clear();
	//             //    }
	//             //    //swap slots (also works if occupying slot is empty)
	//             //    std::swap(*occupyingSlot, stackHolding);
	//             //}
	//         }
	//     }
	// } else {
	//     itemStack *s = selectedSlotContainer->getSlot(selectedSlot);
	//     divider.stackHolding->addStack(*s);
	// }
}

void inventory::drawExtraData(const resolutionTexture& sprite, cveci2& position, cmat3x3& transform, const gameRenderData& targetData) const
{
	drawExtraData(sprite, crectangle2(sprite.getClientRect()), position, transform, targetData);
}

void inventory::drawExtraData(const resolutionTexture& sprite, crectangle2& textureRect, cvec2& position, cmat3x3& transform, const gameRenderData& targetData) const
{
	fillTransparentRectangle(textureRect, mat3x3::cross(transform, mat3x3::translate(cvec2(position))), sprite, targetData.renderTarget);
}

void inventory::drawExtraData(cmat3x3& transform, const gameRenderData& targetData)
{
}

inventory::~inventory()
{
	for (uiSlotContainer* container : containers)
	{
		delete container;
		container = nullptr;
	}
}

void inventory::mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider)
{
	human* currentHuman = (human*)linkedPlayer;
	// check which inventory slot is clicked on
	// convert to 'hotbar slot space'
	veci2 selectedItemPosition;
	uiSlotContainer* selectedSlotContainer = getSlotContainer(pixelPosition, selectedItemPosition);
	if (selectedSlotContainer)
	{
		clickedOnItem(button, divider, selectedSlotContainer, selectedItemPosition);
	}
	else if (divider.stackHolding.count)
	{ // not dividing anything, else stackholding would be empty
		// outside of the slotcontainers
		// drop on the ground
		currentHuman->drop(divider.stackHolding);
		divider = stackDivider();
	}
}

void inventory::drawToolTips(cveci2& mouseTexturePosition, cveci2& mousePositionPixels,
	const texture& renderTarget)
{
	// check which inventory slot is hovered above
	// convert to 'hotbar slot space'
	veci2 selectedItemPosition;
	uiSlotContainer* container = getSlotContainer(mouseTexturePosition, selectedItemPosition);
	if (container)
	{
		itemStack* stack = container->linkedContainer->getSlot(selectedItemPosition);
		if (stack->count)
		{
			stack->drawToolTips(mousePositionPixels, renderTarget);
		}
	}
}

bool inventory::canAddStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	return true;
}

bool inventory::canAddStack(uiSlotContainer* containerToAddTo, cveci2& position, itemStack& s)
{
	return canAddStack(containerToAddTo, s);
}

bool inventory::addStack(uiSlotContainer* containerToAddTo, itemStack& s)
{
	if (canAddStack(containerToAddTo, s)) {
		return containerToAddTo->addStack(s);
	}
	return false;
}

void inventory::linkUp(inventoryForm* form)
{
	form->inventoryToDisplay = this;
	linkedPlayer = ((gameControl*)form->parent)->player;
	form->updateScale();
	((gameControl*)form->parent)->focusChild(form);
	form->visible = true;
}

uiSlotContainer* inventory::getSlotContainer(cveci2& mousePositionPixels, veci2& slotPosition)
{
	// check which inventory slot is hovered above
	// convert to 'hotbar slot space'
	for (uiSlotContainer* const& container : containers)
	{
		if (container->linkedContainer->getSlot(mousePositionPixels,
			container->containerPixelPosition, inventorySpacing,
			inventoryItemDisplaySize, slotPosition))
		{
			return container; // no other slotcontainer will have items at the same place
		}
	}
	return nullptr;
}

void inventory::mouseMove(cveci2& pixelPosition, cmb& button, stackDivider& divider)
{
	if (!divider.pickingUp)
	{
		veci2 selectedItemPosition;
		uiSlotContainer* selectedSlotContainer = getSlotContainer(pixelPosition, selectedItemPosition);
		if (selectedSlotContainer)
		{
			itemStack* s = selectedSlotContainer->getSlot(selectedItemPosition);
			if (s && std::find(divider.divideOver.begin(), divider.divideOver.end(), s) ==
				divider.divideOver.end())
			{
				// new stack to potentially add
				clickedOnItem(button, divider, selectedSlotContainer, selectedItemPosition);
			}
		}
	}
}
