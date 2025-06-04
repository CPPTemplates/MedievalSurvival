#include "tnt.h"
#include "dimension.h"
#include "block.h"
#include "include/math/graphics/graphicsFunctions.h"
#include "include/math/graphics/brush/brushes.h"
tnt::tnt() : entity(entityID::tnt)
{

}

void tnt::tick()
{
	entity::tick();
	if (ticksUntilFuse <= 0)
	{
		//explode
		despawn = true;
		dimensionIn->createExplosion(position, 4);
	}
	else
	{
		ticksUntilFuse--;
	}
}

void tnt::render(const gameRenderData& targetData) const
{
	//render white or tnt texture
	crectangle2 rect = calculateHitBox();
	if (ticksUntilFuse % ticksPerRealLifeSecond < (ticksPerRealLifeSecond / 2))
	{
		//white
		fillTransformedRectangle(targetData.renderTarget, rect, targetData.worldToRenderTargetTransform, brushes::white);
	}
	else
	{
		//tnt texture
		fillTransformedBrushRectangle(crectangle2(blockTextureRect), rect, targetData.worldToRenderTargetTransform, *blockList[blockID::tnt]->tex, targetData.renderTarget);
	}
}

void tnt::serializeMembers(nbtSerializer& s)
{
	entity::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"ticks until fuse"), ticksUntilFuse);
}