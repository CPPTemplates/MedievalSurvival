#pragma once
#include "entity.h"
#include "particleID.h"
//particle
constexpr fp particleSize = 0.25;
constexpr rectangle2 relativeParticleHitbox = crectangle2(vec2(particleSize * -0.5, 0), vec2(particleSize));
struct particle :public entity
{
	int ticksToExist = particleDespawnTicks;

	particle(const entityID& entityType);
	particle(const particleID& particleType);
	void tick() override;
	virtual void render(const gameRenderData& targetData) const override;
	virtual void serializeMembers(nbtSerializer& s) override;
};