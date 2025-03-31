
#include "effectParticle.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/resolutiontexture.h"
#include "math/mattnxn.h"
#include "math/rectangle/rectangletn.h"
#include "math/graphics/brush/brushes/colorMultiplier.h"
#include "gameRenderData.h"

void effectParticle::render(const gameRenderData& targetData) const
{
	const resolutionTexture& tex = getTexture();
	fillTransparentRectangle(tex.getClientRect(), mat3x3::cross(targetData.worldToRenderTargetTransform, mat3x3::fromRectToRect(tex.getClientRect(), calculateHitBox())),
		colorMultiplier<solidColorBrush, resolutionTexture>(solidColorBrush(effectColor), tex), targetData.renderTarget);
}
