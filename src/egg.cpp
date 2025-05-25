#include "egg.h"
#include "math/random/random.h"
#include "dimension.h"
#include "chicken.h"

egg::egg() : throwable(entityID::egg)
{
}
void egg::onCollision(const std::vector<entity*>& collidingEntities)
{
	addImpactDamage(collidingEntities, 0);
	int r = rand(currentRandom, 0x100);
	//1 in 0x100 chance to spawn 2 chickens
	int chickensToSpawn = r == 0 ? 2 : ((r & 0xf) == 0) ? 1 : 0;

	for (int i = 0; i < chickensToSpawn; i++) {
		chicken* babyChicken = new chicken();
		babyChicken->setAge(0);

		if (!fitEntity(babyChicken, dimensionIn, position)) {
			delete babyChicken;
		}

	}

	despawn = true;
}