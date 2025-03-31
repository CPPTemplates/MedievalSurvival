#include "fireBall.h"
fireBall::fireBall(const entityID &entityType) : throwable(entityType)
{
}

void fireBall::onCollision(const std::vector<entity *> &collidingEntities)
{
	addImpactDamage(collidingEntities, fireBallImpactDamage);

	despawn = true;
}

fp fireBall::getGravityForce() const
{
	return 0;
}

std::vector<vec3> fireBall::getFrictions() const
{
	return std::vector<vec3>();
}

void fireBall::tick()
{
	throwable::tick();
	ticksToLive--;
	if (ticksToLive == 0)
	{
		despawn = true;
	}
}
