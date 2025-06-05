#pragma once
#include "taskAI.h"
struct WorkAI : taskAI {
	using taskAI::taskAI;
	virtual bool shouldExecute() override;
	virtual void updateTask() override;
};