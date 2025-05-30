#include "mobData.h"
#include "chicken.h"
#include <string>
#include "constants.h"
#include "entityData.h"
#include "entityID.h"
#include "experienceDrop.h"
#include "globalFunctions.h"
#include "math/bodypart2d.h"
#include "math/random/random.h"
#include "math/vector/vectn.h"
#include "mob.h"
#include "nbt/nbtSerializer.h"
#include "passiveMob.h"
#include "nbt/nbtSerializer.h"
#include "floatingSlot.h"
#include "dimension.h"
#include "soundList.h"

constexpr fp chickenAirGravityForce = 0.4;
chicken::chicken() : mob(entityID::chicken), passiveMob(entityID::chicken)
{
	resetEggLayTime();

	// intialize body parts
	mainBodyPart = new bodyPart2D(chickenBodyTextureRect, nullptr, vec2(position.x, position.y + chickenUpperLegSize.y), chickenBodySize, chickenBodyRotationCenter, 0, 90);

	rightUpperLeg = new bodyPart2D(chickenUpperLegTextureRect, mainBodyPart, cvec2(), chickenUpperLegSize, chickenUpperLegRotationCenter);
	rightLowerLeg = new bodyPart2D(chickenLowerLegTextureRect, rightUpperLeg, cvec2(chickenUpperLegSize.x * 0.5, 0) - chickenUpperLegRotationCenter, chickenLowerLegSize, chickenLowerLegRotationCenter, 0, 90);

	leftUpperLeg = new bodyPart2D(chickenUpperLegTextureRect, mainBodyPart, cvec2(), chickenUpperLegSize, chickenUpperLegRotationCenter, 0, 90);
	leftLowerLeg = new bodyPart2D(chickenLowerLegTextureRect, leftUpperLeg, cvec2(chickenUpperLegSize.x * 0.5, 0) - chickenUpperLegRotationCenter, chickenLowerLegSize, chickenLowerLegRotationCenter, 0, 90);

	rightWing = new bodyPart2D(chickenWingTextureRect, mainBodyPart, cvec2(chickenBodySize.x * 0.5, chickenBodySize.y) - chickenBodyRotationCenter, chickenWingSize, chickenWingRotationCenter);
	leftWing = new bodyPart2D(chickenWingTextureRect, mainBodyPart, cvec2(chickenBodySize.x * 0.5, chickenBodySize.y) - chickenBodyRotationCenter, chickenWingSize, chickenWingRotationCenter);

	head = new bodyPart2D(chickenHeadTextureRect, mainBodyPart, vec2(chickenBodySize.x, chickenBodySize.y) - chickenBodyRotationCenter, chickenHeadSize, chickenHeadRotationCenter);

	bodyPart2D *wattles = new bodyPart2D(chickenWattlesTextureRect, head, cvec2(chickenHeadSize.x, chickenWattlesSize.y) - chickenHeadRotationCenter, chickenWattlesSize, chickenWattlesRotationCenter);
	bodyPart2D *beak = new bodyPart2D(chickenBeakTextureRect, head, cvec2(chickenHeadSize.x, chickenWattlesSize.y + chickenBeakSize.y * 0.5) - chickenHeadRotationCenter, chickenWattlesSize, chickenBeakRotationCenter);

	head->children.push_back(wattles);
	head->children.push_back(beak);

	rightUpperLeg->children.push_back(rightLowerLeg);
	leftUpperLeg->children.push_back(leftLowerLeg);

	mainBodyPart->children.push_back(leftWing);
	mainBodyPart->children.push_back(leftUpperLeg);
	mainBodyPart->drawOrderIndex = mainBodyPart->children.size();
	mainBodyPart->children.push_back(head);
	mainBodyPart->children.push_back(rightUpperLeg);
	mainBodyPart->children.push_back(rightWing);
}

void chicken::updateBodyParts() const
{
	mainBodyPart->translate = position + cvec2(0, leftLowerLeg->size.y + leftUpperLeg->size.y);

	cfp rightLegangle = ((mobData *)entityDataList[(int)entityType])->legSwingSynchronizer.getSineAmpAt(totalLegDistance);

	rightUpperLeg->angle = rightLegangle;
	leftUpperLeg->angle = -rightLegangle;

	rightLowerLeg->angle = rightUpperLeg->angle * -0.5;
	leftLowerLeg->angle = leftUpperLeg->angle * -0.5;

	rightUpperLeg->changed = true;
	leftUpperLeg->changed = true;
	rightLowerLeg->changed = true;
	leftLowerLeg->changed = true;
	mainBodyPart->changed = true;
}

void chicken::serializeMembers(nbtSerializer &s)
{
	mob::serializeMembers(s);
	s.serializeMembers(L"time until next egg", timeUntilNextEgg);
}

fp chicken::getGravityForce() const
{
	return ((fluidArea > 0) || (!flying)) ? mob::getGravityForce() : chickenAirGravityForce;
}

bool chicken::goToPosition(cvec2 &destination)
{
	flying = !onGround && (speed.y < 0);
	return mob::goToPosition(destination);
}

void chicken::tick()
{
	passiveMob::tick();
	if (timeUntilNextEgg)
	{
		timeUntilNextEgg--;
	}
	else
	{
		updateBodyParts();
		popSound->playRandomSound(dimensionIn, mainBodyPart->translate);
		// lay egg
		floatingSlot *egg = (floatingSlot *)summonEntity(entityID::item, dimensionIn, mainBodyPart->translate);
		egg->stack = itemStack(itemID::egg, 1);
		// make it 'eject' from the back
		constexpr fp eggEjectionSpeed = 1; // ejected at 1 m / s
		egg->speed = speed + vec2(mainBodyPart->flipX ? -eggEjectionSpeed : eggEjectionSpeed, 0);
		resetEggLayTime();
	}
}

bool chicken::addDamageSource(cfp &damage, std::shared_ptr<damageSource> source)
{
	//stop laying eggs for a while. it's too anxious to lay an egg
	resetEggLayTime();
    return passiveMob::addDamageSource(damage, source);
}

void chicken::resetEggLayTime()
{
	timeUntilNextEgg = rand(currentRandom, minimalChickenEggTime, maximalChickenEggTime);
}
