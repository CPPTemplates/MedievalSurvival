#pragma once
#include "fireBall.h"
struct ghastFireBall : public fireBall
{
	ghastFireBall();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};