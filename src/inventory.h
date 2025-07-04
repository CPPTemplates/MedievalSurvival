#pragma once
#include "math/graphics/resolutiontexture.h"
#include "slotContainer.h"
#include "interface/idestructable.h"
#include "uiSlotContainer.h"
struct stackDivider;

constexpr rectanglei2 defaultUIRectangle = crectanglei2(0, 90, 176, 166);
struct inventory : slotContainer, IDestructable
{
	human* linkedPlayer = nullptr;
	resolutionTexture* uiTexture = nullptr;
	rectanglei2 uiTexturePartToDraw = defaultUIRectangle;//default

	//containers in order of what to send to first when shift clicking on an item
	std::vector<uiSlotContainer*> containers = std::vector<uiSlotContainer*>();
	//the positions are default, but can be changed by for example the trading slot container
	uiSlotContainer* hotbarSlots = new uiSlotContainer(cveci2(0x8, 0x8), nullptr);
	uiSlotContainer* inventorySlots = new uiSlotContainer(cveci2(0x8, 30), nullptr);

    std::vector<itemStack*> slotsDividingOver = std::vector<itemStack*>();

	virtual void clickedOnItem(cmb& button, stackDivider& divider, uiSlotContainer* selectedSlotContainer, veci2 selectedSlot);
	//void drawExtraData(crectanglei2& textureRect, cveci2& offset, cmat3x3& transform, const texture& renderTarget) const;
	void drawExtraData(const resolutionTexture& sprite, cveci2& position, cmat3x3& transform, const gameRenderData& targetData) const;
	void drawExtraData(const resolutionTexture& sprite, crectangle2& textureRect, cvec2& position, cmat3x3& transform, const gameRenderData& targetData) const;
	virtual void drawExtraData(cmat3x3& transform, const gameRenderData& targetData);
	virtual ~inventory() override;
	virtual void mouseDown(cveci2& pixelPosition, cmb& button, stackDivider& divider);
    virtual void mouseMove(cveci2& pixelPosition, cmb& button, stackDivider& divider);
	virtual void drawToolTips(cveci2& mouseTexturePosition, cveci2& mousePositionPixels, const texture& renderTarget);
	//returns true if the stack we're holding is a proper stack for the container we're going to interact with
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, itemStack& s);
	//for armor slots, it'll only return true if it's the right armor piece
	virtual bool canAddStack(uiSlotContainer* containerToAddTo, cveci2& position, itemStack& s);
	virtual bool addStack(uiSlotContainer* containerToAddTo, itemStack& s);
	virtual void linkUp(struct inventoryForm* form);

	uiSlotContainer* getSlotContainer(cveci2& mousePositionPixels, veci2& slotPosition);
};