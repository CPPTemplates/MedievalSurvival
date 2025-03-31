#include "passiveMob.h"
#include "panicAI.h"
#include "wanderAI.h"
#include "taskList.h"
#include "loveModeAI.h"
#include "lureAI.h"
passiveMob::passiveMob(const entityID& entityType) : mob(entityType)
{
	this->tasks = new taskList(this, {
		new panicAI(this),
		new loveModeAI(this),
		new lureAI(this),
		new wanderAI(this),
		});
}