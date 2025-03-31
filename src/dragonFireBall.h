#pragma once
#include "fireBall.h"
struct dragonFireBall : public fireBall
{
	dragonFireBall();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};