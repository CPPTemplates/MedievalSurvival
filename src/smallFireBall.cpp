#include "smallFireBall.h"
smallFireBall::smallFireBall() :fireBall(entityID::small_Fireball)
{

}

void smallFireBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	fireBall::onCollision(collidingEntities);

	addImpactDamage(collidingEntities, 5);

	for (entity* e : collidingEntities)
	{
		e->setOnFire(100);
	}
}