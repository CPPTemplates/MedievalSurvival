#pragma once
#include "math/graphics/resolutiontexture.h"
#include "particle.h"
struct rectParticle : particle
{
	rectangle2 brushRect;
	using particle::particle;
	rectParticle(const particleID& particleType, crectangle2& brushRect = rectangle2()) : brushRect(brushRect), particle(particleType) {}
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual const resolutionTexture& getTexture() const = 0;
};
rectangle2 getParticleBrushRect(const resolutionTexture& tex);