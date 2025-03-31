#include "loveModeAI.h"
#include "nbt/serializeUUID.h"
#include "mob.h"
#include "dimension.h"
#include "taskList.h"
#include "particle.h"
#include "iconParticle.h"
constexpr fp loveModeRadius = (fp)5;
bool loveModeAI::shouldExecute()
{
	mob* connectedMob = (mob*)connectedEntity;
	if (connectedMob->ticksInLove < 0)
		connectedMob->ticksInLove++;
	else if (connectedMob->ticksInLove > 0) {
		if (!connectedMob->partnerFound) {
			//search for nearby entities in love
			for (entity* e : connectedMob->dimensionIn->findNearEntities(connectedMob->position, loveModeRadius)) {
				if (e->entityType == connectedMob->entityType && ((mob*)e)->ticksInLove > 0 && e->identifier != connectedMob->identifier) {
					((mob*)e)->partnerFound = connectedMob->partnerFound;
					connectedMob->partnerFound = e->identifier;
					break;
				}
			}
		}
	}

	return connectedMob->partnerFound;
}

void loveModeAI::updateTask()
{
	mob* connectedMob = (mob*)connectedEntity;
	//the radius at which they can produce a baby
	constexpr fp breedRadius = (fp)1;

	mob* partner = (mob*)connectedMob->dimensionIn->findUUID(connectedMob->position, loveModeRadius + 1, connectedMob->partnerFound);
	if (!partner) {
		//cannot find partner. will look for a new partner next tick
		connectedMob->partnerFound = uuid();
		return;
	}
	connectedMob->goToPosition(partner->position);
	if ((partner->position - connectedMob->position).lengthSquared() < math::squared(breedRadius)) {
		//the center of the baby hitbox when nothing obstructs it
		vec2 babyHitboxExpandPosition = (connectedMob->calculateHitBox().getCenter() + partner->calculateHitBox().getCenter()) * (fp)0.5;
		mob* baby = (mob*)createEntity(connectedMob->entityType);
		baby->setAge(0);
		vec2 resultPosition = vec2();
		if (connectedMob->dimensionIn->fitExpandingHitbox(baby->relativeHitbox, babyHitboxExpandPosition, resultPosition)) {
			summonEntity(baby, connectedMob->dimensionIn, resultPosition);
		}
		else {
			delete baby;
			baby = nullptr;
		}

		for (int i = 0; i < 3; i++) {
			summonEntity(new iconParticle(baby ? particleID::heart : particleID::angry_villager),connectedMob->dimensionIn, babyHitboxExpandPosition);
		}

		constexpr int resetValue = -6 * ticksPerHour;
		partner->ticksInLove = resetValue;
		connectedMob->ticksInLove = resetValue;
		connectedMob->partnerFound = uuid();
		partner->partnerFound = uuid();
		return;
	}
	((mob*)connectedEntity)->ticksInLove--;
}

void loveModeAI::serializeValue(nbtSerializer& s)
{
}