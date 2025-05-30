#pragma once
#include "throwable.h"
constexpr int enderEyeFloatTicks = 0x50;
struct eyeOfEnder : public throwable
{
	//the amount of ticks the ender eye still has to float
	int floatTicksLeft = enderEyeFloatTicks;
	vec2 strongholdPosition = vec2();
	eyeOfEnder();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
	virtual void tick() override;
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual vec2 applyNaturalForces(cvec2& speed) const override;
};