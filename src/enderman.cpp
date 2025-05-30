#include "dimension.h"
#include "enderman.h"
#include "taskList.h"
#include "teleportOnDamageAI.h"
#include "wanderAI.h"
#include "stareDownAI.h"

enderman::enderman() :humanoid(entityID::enderman)
{
	tasks = new taskList(this, {
	new teleportOnDamageAI(this),
	new stareDownAI(this),
	new wanderAI(this),
		});

	initializeBodyParts(endermanHeadTextureRect, endermanBodyTextureRect, endermanLeftLegTextureRect, endermanRightLegTextureRect, endermanLeftArmTextureRect, endermanRightArmTextureRect, endermanPixelSize);
	itemHolding = new itemStack();
}

void enderman::serializeMembers(nbtSerializer& s)
{
	humanoid::serializeMembers(s);
	itemHolding->serialize(s, std::wstring(L"item holding"));
}
bool enderman::addDamageSource(cfp& damage, std::shared_ptr<damageSource> source)
{
	if (source.get()->type == projectileDamage)
	{
		return false;
	}
	else
	{
		return humanoid::addDamageSource(damage, source);
	}
}