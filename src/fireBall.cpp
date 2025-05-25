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

vec2 fireBall::applyNaturalForces(cvec2& speed) const
{
	return speed;
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
