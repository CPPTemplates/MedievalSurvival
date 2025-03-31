#pragma once
#include "taskAI.h"
#include "followAI.h"
//follows and attacks the target entity
struct targetAI : followAI
{
	virtual bool shouldExecute() override;
	bool targetNear() const;
	targetAI(entity* connectedEntity, cfp& proximity = standardDetectionRange) :followAI(connectedEntity, proximity) {}
	virtual void updateTask() override;
};