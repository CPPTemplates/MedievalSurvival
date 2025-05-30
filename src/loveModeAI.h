#pragma once
#include "taskAI.h"
struct loveModeAI : taskAI
{
	virtual bool shouldExecute() override;
	virtual void updateTask() final;
	//virtual void updateTask();
	loveModeAI(entity* const& connectedEntity) : taskAI(connectedEntity) {}
	virtual void serializeMembers(nbtSerializer& s) override;
};