#include "dimension.h"
#include "zombie.h"
#include "taskList.h"
#include "attackOnCollideAI.h"
#include "wanderAI.h"
#include "hurtByTargetAI.h"
zombie::zombie(const entityID& entityType) : humanoid(entityType)
{
	tasks = new taskList(this, {
								   new hurtByTargetAI(this),
								   new attackOnCollideAI(this, entityID::human),
								   new wanderAI(this),
		});

	initializeBodyParts(zombieHeadTextureRect, zombieBodyTextureRect, zombieLeftLegTextureRect, zombieRightLegTextureRect, zombieLeftArmTextureRect, zombieRightArmTextureRect, humanoidPixelSize);
	itemHolding = new itemStack();
}
void zombie::serializeMembers(nbtSerializer& s)
{
	humanoid::serializeMembers(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}