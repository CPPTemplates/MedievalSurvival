#include "textureParticle.h"
#include "animatedParticleData.h"
#include "particleTypeData.h"
#include "world.h"
#include "include/math/graphics/brush/brushes.h"
void textureParticle::render(const gameRenderData& targetData) const
{
	const resolutionTexture& tex = getTexture();
	fillTransparentTexture(mat3x3::cross(targetData.worldToRenderTargetTransform, mat3x3::fromRectToRect(tex.getClientRect(), calculateHitBox())), tex, targetData.renderTarget);
}

const resolutionTexture& textureParticle::getTexture() const
{
	const animatedParticleData* data = (const animatedParticleData*)particleTypeDataList[(particleID)((int)entityType - (int)entityID::particle)];
	return *data->animatedTextures[currentWorld->ticksSinceStart % data->animatedTextures.size()];
}
