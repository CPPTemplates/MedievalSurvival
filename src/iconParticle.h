#pragma once
#include "particle.h"
#include "textureParticle.h"
struct iconParticle final : textureParticle {
	inline iconParticle(const particleID& particleType) : textureParticle(particleType) {
		ticksToExist = 2 * ticksPerRealLifeSecond;
		//float 1 block up / second
		speed.y = 1 * secondsPerTick;
	}
	virtual vec2 applyNaturalForces(cvec2& speed) const final;
	virtual fp getGravityForce() const final;
};