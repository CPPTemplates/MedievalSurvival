#include "wanderAI.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
#include "nbt/serializeVector.h"
#include "dimension.h"
bool wanderAI::continueExecuting()
{

	return true;
}

void wanderAI::startExecuting()
{
	targetPos = connectedEntity->position;
}

void wanderAI::updateTask()
{
	mob* connectedMob = (mob*)connectedEntity;
	cbool& reachedTarget = abs(targetPos.x - connectedEntity->position.x) < 0.5;
	cbool& canReachTargetPoint = reachedTarget || connectedMob->goToPosition(targetPos);
	int averageStandTime = 10 * ticksPerRealLifeSecond;
	if (randChance(currentRandom,
		reachedTarget ? averageStandTime : //standing still before going to a new random position
		canReachTargetPoint ? 20 * ticksPerRealLifeSecond : //walking to target point
		5 * ticksPerRealLifeSecond))//can'T reach target point somehow, let's choose another point
	{
		cfp targetOffset = 0x10;
		//choose new target
		vec2 newTargetPos = vec2(connectedEntity->position.x + randFp(currentRandom, -targetOffset, targetOffset), connectedEntity->position.y);
		//check if chunk is loaded at new position. do not walk into unloaded chunks randomly!
		if (connectedMob->dimensionIn->getLoadLevel(floorVector(newTargetPos)) >= chunkLoadLevel::entityLoaded) {
			targetPos = newTargetPos;
		}
	}

	connectedMob->flipBodyToWalkingDirection();
	connectedMob->lookForward();
}

void wanderAI::resetTask()
{
	targetPos = connectedEntity->position;
}

wanderAI::wanderAI(entity* connectedEntity) : taskAI(connectedEntity)
{
	resetTask();
}

void wanderAI::serializeMembers(nbtSerializer& s)
{
	serializeNBTValue(s, L"target position", targetPos);
}