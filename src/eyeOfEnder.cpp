#include "eyeOfEnder.h"
#include "itemDrop.h"
#include "dimension.h"
#include "soundList.h"
#include "structureID.h"
#include "nbt/nbtSerializer.h"
#include "math/random/random.h"
constexpr fp maxEyeVelocity = 3;
void eyeOfEnder::tick()
{
	if (floatTicksLeft == enderEyeFloatTicks) {
		//calculate distance to the nearest stronghold
		dimensionIn->locateStructure(structureID::stronghold, position, strongholdPosition);
	}
	if (floatTicksLeft >= 0x20)
	{
		cfp dx = strongholdPosition.x - position.x;
		cfp distance = abs(dx);

		//nearer than this amount of blocks, the eye goes down
		cfp digDistance = 0x10;

		//float up if it is far, down if it is nearby
		cfp upwardsVelocity = math::maximum((fp)((1.0 - ((digDistance / distance) * 2.0)) * maxEyeVelocity), (fp)(-maxEyeVelocity));

		//float faster if it is farther away
		cfp sideVelocity = math::maximum((1.0 - (digDistance / distance)) * maxEyeVelocity, (fp)0.0);
		cfp signedSideVelocity = dx < 0 ? -sideVelocity : sideVelocity;
		speed = cvec2(signedSideVelocity, upwardsVelocity);
	}
	else
	{
		if (floatTicksLeft >= 0x18)
		{
			//brake
			cfp brakeFriction = 0.9;
			speed *= brakeFriction;
		}
		else
		{
			if (floatTicksLeft > 0)
			{
				//wobble in the air down, up, down
				if (floatTicksLeft % 0x10 >= 0x8)
				{
					speed.y += maxEyeVelocity;
				}
				else
				{
					speed.y -= maxEyeVelocity;
				}
			}
			else
			{
				//break or drop
				despawn = true;
				if (!randChance(currentRandom, 5))
				{
					//drop as item
					itemDrop::dropLoot({ itemStack(itemID::ender_eye, 1) }, dimensionIn, position);
				}
				eyeOfEnderDropSound->playRandomSound(dimensionIn, position);
			}
		}
	}

	floatTicksLeft--;
}

void eyeOfEnder::serializeMembers(nbtSerializer& s)
{
	//the stronghold direction will be calculated
	serializeNBTValue(s, std::wstring(L"ticks to float"), floatTicksLeft);
}

vec2 eyeOfEnder::applyNaturalForces(cvec2& speed) const
{
	return speed;
}
eyeOfEnder::eyeOfEnder() : throwable(entityID::ender_eye)
{
}

void eyeOfEnder::onCollision(const std::vector<entity*>& collidingEntities)
{

}