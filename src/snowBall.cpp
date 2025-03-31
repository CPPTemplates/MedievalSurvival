#include "snowBall.h"
snowBall::snowBall() : throwable(entityID::snowball)
{

}
void snowBall::onCollision(const std::vector<entity*>& collidingEntities)
{
	addImpactDamage(collidingEntities, 0);
	despawn = true;
}