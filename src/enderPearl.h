#pragma once
#include "throwable.h"
struct enderPearl : public throwable
{
	//uuid uuidTeleporting = uuid();
	enderPearl();
	virtual void onCollision(const std::vector<entity*>& collidingEntities) override;
};