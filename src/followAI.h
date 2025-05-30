#pragma once
#include "taskAI.h"
constexpr fp standardDetectionRange = 0x10;
//follows the target entity (walks to it)
struct followAI : taskAI {
	//proximity will be set by the initializer and doesn'T have to be serialized
	fp proximity = 0;
	uuid target = uuid();
	bool canSeeTarget(const entity* targetEntity, fp proximity = 0) const;
	followAI(entity* connectedEntity, fp proximity) : taskAI(connectedEntity), proximity(proximity) {}
	virtual bool shouldExecute() override;
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual void updateTask() override;
	entity* findTarget() const;
};
