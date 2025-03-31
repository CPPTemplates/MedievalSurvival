#pragma once
#pragma once
#include "followAI.h"

constexpr fp followRadius = 0x8;
struct lureAI : followAI {
	bool shouldExecute() override;
	lureAI(entity* connectedEntity) : followAI(connectedEntity, followRadius) {}
};