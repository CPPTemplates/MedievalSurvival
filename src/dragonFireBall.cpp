#include "dragonFireBall.h"
#include "soundList.h"
#include "dimension.h"
dragonFireBall::dragonFireBall() :fireBall(entityID::dragon_fireball)
{
}

void dragonFireBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	fireBall::onCollision(collidingEntities);
	//create lingering potion area
	explosionSound->playRandomSound(dimensionIn, position);
}