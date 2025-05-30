#include "blazeFireBallAI.h"
#include "soundList.h"
#include "dimension.h"
#include "nbt/nbtSerializer.h"
bool blazeFireBallAI::shouldExecute()
{
	if (fireBallCoolDown == 0 || fireBallsLeft)
	{
		if (connectedEntity->fireTicks == 0)
		{
			fireSound->playRandomSound(connectedEntity->dimensionIn, connectedEntity->position);
		}
		connectedEntity->fireTicks = math::maximum(connectedEntity->fireTicks, 0x2);
	}

	if (fireBallCoolDown)
	{
		fireBallCoolDown--;
		return false;
	}
	else
	{
		if (fireBallsLeft)
		{
			return attackOnCollideAI::shouldExecute();
		}
		else if (rand() % 0x10)
		{
			fireBallsLeft = 3;
			return attackOnCollideAI::shouldExecute();
		}
		else
		{
			return false;
		}
	}
}

bool blazeFireBallAI::continueExecuting()
{
	return shouldExecute();
}
void blazeFireBallAI::updateTask()
{
	if (const entity* target = findTarget()) {
		shootFireBall(target);
		fireBallsLeft--;
		if (fireBallsLeft <= 0)
		{
			fireBallCoolDown = 100;
		}
		else
		{
			fireBallCoolDown = 5;
		}
	}
}

void blazeFireBallAI::serializeMembers(nbtSerializer& s)
{
	fireBallAI::serializeMembers(s);
	s.serializeMembers(std::wstring(L"fireball cooldown"), fireBallCoolDown);
	s.serializeMembers(std::wstring(L"fireballs left"), fireBallsLeft);
}