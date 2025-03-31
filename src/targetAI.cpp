#include "targetAI.h"
#include "mob.h"
#include "dimension.h"
#include "human.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeUUID.h"
bool targetAI::shouldExecute()
{
	return targetNear();
}

bool targetAI::targetNear() const
{
	if (target)
	{
		entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target);
		if (targetEntity)
		{
			return canSeeTarget(targetEntity);
		}
	}
	return false;
}



void targetAI::updateTask()
{
	if (entity* targetEntity = connectedEntity->dimensionIn->findUUID(connectedEntity->position, proximity + mobSizeMargin, target)) {

		cfp& distanceToTargetSquared = (targetEntity->position - connectedEntity->position).lengthSquared();
		if (targetEntity && (distanceToTargetSquared < math::squared(proximity)))
		{
			mob* connectedMob = (mob*)connectedEntity;
			//can detect a player from this far away

			if (((mob*)connectedEntity)->ticksSinceToolUsed >= ((mob*)connectedEntity)->getTotalCoolDownTime())
			{
				//performance heavy
				connectedMob->updateSelection();
				if (connectedMob->selectedUUID == targetEntity->identifier)
				{
					connectedMob->wantsToHit = true;
				}
				else if (connectedMob->selectedUUID)
				{
					//tries to jump to hit the player over other entities
					connectedMob->wantsToJump = true;
				}
			}
			cbool couldHitTarget = connectedMob->selectedUUID == targetEntity->identifier;
			if (distanceToTargetSquared > math::squared(2) || !couldHitTarget)
			{
				connectedMob->goToPosition(targetEntity->position);
			}

			connectedMob->flipBodyToWalkingDirection();
			connectedMob->lookAtEntity(targetEntity);
		}
	}
}
