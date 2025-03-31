#pragma once
#include "particle.h"
#include "math/graphics/resolutiontexture.h"
#include "math/rectangle/rectangletn.h"
#include "particleID.h"
#include "gameRenderData.h"
struct textureParticle : particle
{
	using particle::particle;
	size_t brushIndex = 0;
	virtual void render(const gameRenderData& targetData) const override;
	const resolutionTexture& getTexture() const;
};
