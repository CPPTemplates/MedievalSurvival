#pragma once
#include "throwable.h"
struct fireBall : public throwable
{
	fireBall(const entityID& entityType);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
	virtual fp getGravityForce() const override;
	virtual vec2 applyNaturalForces(cvec2& speed) const override;
	void tick() override;
	int ticksToLive = 0x8 * ticksPerRealLifeSecond;
};