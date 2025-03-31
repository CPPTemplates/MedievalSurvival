#pragma once
#include "throwable.h"
struct snowBall : public throwable
{
	snowBall();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};