#include "followAI.h"
#include "dimension.h"
#include <humanoid.h>
#include "idAnalysis.h"
#include <nbt/serializeUUID.h>
#include "human.h"

bool followAI::shouldExecute()
{

	return false;
}
bool followAI::canSeeTarget(const entity* targetEntity, fp proximity) const
{
	if (!proximity)proximity = this->proximity;
	cfp distanceToTargetSquared = (targetEntity->position - connectedEntity->position).lengthSquared();
	if (distanceToTargetSquared < math::squared(proximity))
	{
		if (targetEntity->entityType != entityID::human || willBeNoticedByMobs(((human*)targetEntity)->currentGameMode))
		{

			mob* connectedMob = (mob*)connectedEntity;
			mob* targetMob = (mob*)targetEntity;
			connectedMob->updateBodyParts();
			targetMob->updateBodyParts();
			cvec2 eyePosition = connectedMob->getHeadPosition();
			cvec2& targetHeadPosition = targetMob->getHeadPosition();
			cvec2 relative = targetHeadPosition - eyePosition;
			if (relative.lengthSquared() > math::squared(proximity))
			{
				return false;
			}

			veci2 blockPos, adjacentBlockPosition;
			vec2 exactBlockIntersection;
			tickableBlockContainer* resultingContainer;

			connectedMob->dimensionIn->findRaycastRecursive(eyePosition, targetHeadPosition, blockPos, adjacentBlockPosition, exactBlockIntersection, resultingContainer);
			return exactBlockIntersection == targetHeadPosition;
		}
	}
	return false;
}

void followAI::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, std::wstring(L"target"), target);
}

void followAI::updateTask()
{
	//we assume that we have a target UUID if shouldExecute() is true
	if (entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target)) {

		cfp& distanceToTargetSquared = (connectedEntity->position - targetEntity->position).lengthSquared();
		if (distanceToTargetSquared < math::squared(proximity))
		{
			mob* connectedMob = (mob*)connectedEntity;
			if (distanceToTargetSquared > math::squared(0.5))
			{
				connectedMob->goToPosition(targetEntity->position);
			}
			connectedMob->flipBodyToWalkingDirection();
			connectedMob->lookAtEntity(targetEntity);
		}
	}
}

entity* followAI::findTarget() const
{
	return connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);
}