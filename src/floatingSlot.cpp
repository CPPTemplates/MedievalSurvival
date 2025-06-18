#include "floatingSlot.h"
#include "dimension.h"
#include "itemData.h"
floatingSlot::floatingSlot() :entity(entityID::item)
{
	this->stack = itemStack();
}

void floatingSlot::tick()
{
	//despawns after 5 minutes
	if (ticksAvailable++ >= floatingSlotDespawnTicks)
	{
		despawn = true;
	}

	if (stuck)
	{
		//teleport out of block
		vec2 teleportPosition;
		if (dimensionIn->fitExpandingHitbox(relativeHitbox, crectangle2(newPosition + relativeHitbox.pos0, relativeHitbox.size).expanded(1), newPosition, teleportPosition))
		{
			newPosition = teleportPosition;
		}
	}
	entity::tick();
}

void floatingSlot::render(const gameRenderData& targetData) const
{
	cfp& height = ((fp)std::sin(microsectosec(getmicroseconds()) * math::PI) + 1) * (relativeHitbox.h - itemSize) * 0.5;
	stack.renderSingleItem(targetData.clone(
		mat3x3::fromRectToRect(
			crectangle2(replaceIfMissing(itemList[(int)stack.stackItemID]->tex).getClientRect()),
			targetData.worldToRenderTargetTransform.multRectMatrix(rectangle2(position + relativeHitbox.pos0 + vec2(0, height), vec2(itemSize))))));

	entity::render(targetData);
}

floatingSlot::~floatingSlot()
{

}

void floatingSlot::serializeMembers(nbtSerializer& s)
{
	entity::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"ticks floating"), ticksAvailable);
	stack.serialize(s, std::wstring(L"item"));
}