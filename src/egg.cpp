#include "egg.h"
egg::egg() : throwable(entityID::egg)
{
}
void egg::onCollision(const std::vector<entity*>& collidingEntities)
{
	addImpactDamage(collidingEntities, 0);
	despawn = true;
}