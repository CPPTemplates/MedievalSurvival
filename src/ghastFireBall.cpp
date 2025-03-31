#include "ghastFireBall.h"
#include "dimension.h"
ghastFireBall::ghastFireBall() :fireBall(entityID::ghast_fireball)
{
}

void ghastFireBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	fireBall::onCollision(collidingEntities);
	dimensionIn->createExplosion(position, 1, true);

}