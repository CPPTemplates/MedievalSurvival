#pragma once
#include "taskAI.h"
struct wanderAI : taskAI
{
	vec2 targetPos = vec2();
	virtual bool continueExecuting() override;
	virtual void startExecuting() override;
	virtual void updateTask() override;
	virtual void resetTask() override;
	wanderAI(entity* connectedEntity);
	virtual void serializeMembers(nbtSerializer& s) override;
};