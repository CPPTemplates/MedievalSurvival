#pragma once
#include "fireBall.h"
struct smallFireBall : public fireBall
{
	smallFireBall();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};