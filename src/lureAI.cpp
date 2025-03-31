#include "lureAI.h"
#include "dimension.h"
#include "humanoid.h"
bool lureAI::shouldExecute()
{
	auto shouldFollow = [this](humanoid* h) {
		//calculate if the mob can see the humanoid
		return isBreedItem(connectedEntity->entityType, h->itemHolding->stackItemID) && canSeeTarget(h);
		};
	if (target) {
		if (entity* targetEntity = findTarget()) {
			if (targetEntity && shouldFollow((humanoid*)targetEntity))
				return true;
		}
	}
	target = uuid();
	fp nearestDistanceSquared = INFINITY;

	//follow the closest entity holding a breed item
	for (const entity* const& nearEntity : connectedEntity->dimensionIn->findNearEntities(connectedEntity->position, followRadius)) {
		if (isHumanoid(nearEntity->entityType)) {
			cfp& distanceSquared = (nearEntity->position - connectedEntity->position).lengthSquared();
			if (distanceSquared < nearestDistanceSquared && shouldFollow((humanoid*)nearEntity)) {
				nearestDistanceSquared = distanceSquared;
				target = nearEntity->identifier;
			}
		}
	}
	return false;
}
