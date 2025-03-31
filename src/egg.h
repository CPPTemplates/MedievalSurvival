#pragma once
#include "throwable.h"
struct egg : public throwable
{
	egg();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};