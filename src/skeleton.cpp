#include "skeleton.h"
#include "dimension.h"
#include "humanoid.h"
#include "taskList.h"
#include "arrowAttackAI.h"
#include "wanderAI.h"
#include "hurtByTargetAI.h"
skeleton::skeleton(const entityID &entityType) : humanoid(entityType)
{
	tasks = new taskList(this, {
								   new hurtByTargetAI(this),
								   new arrowAttackAI(this, entityID::human),
								   new attackOnCollideAI(this, entityID::human),
								   new wanderAI(this),
							   });

	initializeBodyParts(skeletonHeadTextureRect, skeletonBodyTextureRect, skeletonLeftLegTextureRect, skeletonRightLegTextureRect, skeletonLeftArmTextureRect, skeletonRightArmTextureRect);
	mainBodyPart->hasTransparency = true;
	itemHolding = new itemStack(itemID::bow, 1);
}

void skeleton::serializeMembers(nbtSerializer &s)
{
	humanoid::serializeMembers(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}