#pragma once
#include "throwable.h"
struct egg : public throwable
{
	egg(dimension* dimensionIn, cvec2& position);
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};