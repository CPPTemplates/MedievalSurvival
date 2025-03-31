#include "rectParticle.h"
#include "nbt/serializeRectangle.h"
#include "math/graphics/brush/brushes.h"
void rectParticle::serializeValue(nbtSerializer& s)
{
	particle::serializeValue(s);
	serializeNBTValue(s, std::wstring(L"brush rect"), brushRect);
}

void rectParticle::render(const gameRenderData& targetData) const
{
	cmat3x3& transForm = mat3x3::combine({ mat3x3::fromRectToRect(brushRect,calculateHitBox()),targetData.worldToRenderTargetTransform });
	fillTransparentRectangle(brushRect, transForm, getTexture(), targetData.renderTarget);
}