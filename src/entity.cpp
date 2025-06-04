#include "entity.h"
#include "math/collisions.h"
#include "entityData.h"
#include "world.h"
#include "dimension.h"
#include "math/vector/vectn.h"
#include "soundList.h"
#include "human.h"
#include "idAnalysis.h"
#include "lightLevel.h"
#include "pressurePlateData.h"
#include "fluidList.h"
#include "block.h"
#include "fluidData.h"

#include "chunk.h"
#include "gameControl.h"
#include <vector>
#include <math.h>
#include <cmath>
#include <map>
#include <memory>
#include <string>
#include "armorType.h"
#include "itemID.h"
#include "chunkLoadLevel.h"
#include "collisionData.h"
#include "collisionDataCollection.h"
#include "collisionEdgeData.h"
#include "collisionTypeID.h"
#include "constants.h"
#include "damageSource.h"
#include "damageType.h"
#include "dimensionID.h"
#include "entityID.h"
#include "humanoid.h"
#include "array/arraynd/arraynd.h"
#include "globalFunctions.h"
#include "math/direction.h"
#include "math/graphics/brush/brushes.h"
#include "math/graphics/color/color.h"
#include "math/mathFunctions.h"
#include "math/rectangle/rectangletn.h"
#include "nbt/nbtData.h"
#include "nbt/nbtDataTag.h"
#include "nbt/nbtSerializer.h"
#include "gameRenderData.h"
#include "statusEffect.h"
#include "statusEffectID.h"
#include "tickableBlockContainer.h"
#include "math/vector/vectorrandom.h"
#include "nbt/serializeVector.h"
#include "nbt/serializeUUID.h"
#include "nbt/serializeRectangle.h"
#include "rectangularSlotContainer.h"
#include "EntityAI.h"
#include "AttachedSound.h"
#include <Linkable.h>

constexpr veci2 endBlockSpawningOn = cveci2(mainEndIslandMaxRadius / 2, 0);
constexpr vec2 endSpawningLocation = cvec2(endBlockSpawningOn.getX() + 0.5, endBlockSpawningOn.getY() + 1 + math::fpepsilon);
constexpr int endPlatformRadius = 0x2;
constexpr int endPlatformSpaceHeight = 0x4;

extern idList<block*, blockID> blockList;

rectangle2 entity::calculateHitBox() const
{
	return calculateHitBox(position);
}
int entity::getEffectLevel(const statusEffectID& id) const
{
	int highestPotency = 0;
	for (const statusEffect& effect : activeEffects)
	{
		if (effect.identifier == id)
		{
			highestPotency = math::maximum(effect.potency, highestPotency);
		}
	}
	return highestPotency;
}
vec2 entity::getRenderOffset(const gameRenderData& targetData) const
{
	// when traveling to another dimension, don'T interpolate positions
	return dimensionIn == newDimension ? targetData.getRenderOffset(position, newPosition) : vec2();
}
void entity::setInitialPosition(tickableBlockContainer* containerIn, cvec2& position)
{
	if (containerIn) {
		this->dimensionIn = containerIn->rootDimension;
		this->position = containerIn->containerToRootTransform.multPointMatrix(position);
	}
	else {
		this->dimensionIn = nullptr;
		this->position = position;
	}
	this->newDimension = this->dimensionIn;
	this->newPosition = this->position;
}
void entity::addSpeed(cvec2& additionalSpeed)
{
	speed += additionalSpeed;
}
vec2 entity::handleCollision(cvec2& otherSpeed, cfp& otherMass)
{
	cfp& weight = getWeight();
	// should collide directly ?
	// not a reference
	cvec2 oldSpeed = speed;
	speed = getSpeedAfterCollision(cvect2<vec2>(oldSpeed, otherSpeed), vec2(weight, otherMass));
	return getSpeedAfterCollision(cvect2<vec2>(otherSpeed, oldSpeed), vec2(otherMass, weight));
}
entity::entity(const entityID& entityType) : identifier(randomUUID(currentRandom))
{
	this->entityType = entityType;

	entityData* data = entityDataList[(int)entityType];

	health = getMaxHealth();
	relativeHitbox = data->initialRelativeHitbox;
}
void entity::tick()
{
	// remove old damage sources
	constexpr int damageRememberTime = 100;
	for (auto it = lastDamageSources.begin(); it != lastDamageSources.end(); it++)
	{
		if (it->get()->damageTick + damageRememberTime < currentWorld->ticksSinceStart)
		{
			it = lastDamageSources.erase(it);
			if (it == lastDamageSources.end())
			{
				break;
			}
		}
	}

	for (size_t i = 0; i < activeEffects.size(); i++)
	{
		statusEffect& effect = activeEffects[i];
		applyStatusEffect(effect);
		effect.ticksDuration--;
		if (effect.ticksDuration <= 0)
		{
			// remove
			if (effect.identifier == statusEffectID::absorption)
			{
				activeEffects.erase(activeEffects.begin() + i);
				fp maximalAbsorptionHealth = 0;
				for (size_t i = 0; i < activeEffects.size(); i++)
				{
					if (activeEffects[i].identifier == statusEffectID::absorption)
					{
						maximalAbsorptionHealth = math::maximum(maximalAbsorptionHealth, (fp)(activeEffects[i].potency * 4));
					}
				}
				absorptionHealth = math::minimum(absorptionHealth, maximalAbsorptionHealth);
			}
			else
			{
				activeEffects.erase(activeEffects.begin() + i);
			}
			i--;
		}
	}

	if (tasks)
	{
		tasks->execute();
	}

	crectangle2 currentHitbox = calculateHitBox(position);
	if (portalCoolDown)
	{
		portalCoolDown--;
	}

	if (entityDataList[(int)entityType]->canGoThroughPortals)
	{
		if (hitboxContainsOnly(currentHitbox, { blockID::portal }))
		{
			if (portalCoolDown == 0)
			{
				if (portalTicks > 80)
				{
					// travel to other dimension
					dimension* newDimension = dimensionIn->identifier == dimensionID::overworld ? currentWorld->dimensions[(int)dimensionID::nether] : currentWorld->dimensions[(int)dimensionID::overworld];
					// seek nether portal
					veci2 dimensionPosition;
					switch (newDimension->identifier)
					{
					case dimensionID::overworld:
					{
						dimensionPosition = floorVector(position * netherToOverWorldScale);
					}
					break;
					case dimensionID::nether:
					{
						dimensionPosition = floorVector(position / netherToOverWorldScale);
					}
					break;
					default:
						break;
					}
					cveci2& portalBlockPosition = newDimension->searchPortal(dimensionPosition);
					cvec2& teleportPosition = cvec2(portalBlockPosition) + cvec2(0.5, math::fpepsilon);

					teleportTo(newDimension, teleportPosition, false);
					portalArriveSound->playRandomSound(newDimension, teleportPosition);
					portalCoolDown = 80;
				}
				else
				{
					if (portalTicks == 0)
					{
						portalDepartSound->playRandomSound(dimensionIn, position);
					}
					portalTicks++;
				}
			}
		}
		else if (hitboxContains(currentHitbox, { blockID::end_portal }))
		{
			if (newDimension->identifier == dimensionID::overworld)
			{
				// teleporting to the end
				teleportTo(currentWorld->dimensions[(int)dimensionID::end], endSpawningLocation, false);
				// generate obsidian platform
				newDimension->setBlockRange(endBlockSpawningOn + cveci2(-endPlatformRadius, 0), endBlockSpawningOn + cveci2(endPlatformRadius, 0), blockID::obsidian, chunkLoadLevel::entityLoaded);
				newDimension->setBlockRange(endBlockSpawningOn + cveci2(-endPlatformRadius, 1), endBlockSpawningOn + cveci2(endPlatformRadius, endPlatformSpaceHeight), blockID::air, chunkLoadLevel::entityLoaded);

				portalArriveSound->playRandomSound(newDimension, newPosition);
			}
			else
			{
				teleportTo(currentWorld->dimensions[(size_t)currentWorld->worldSpawnDimension], currentWorld->worldSpawnPoint, false);
				if (entityType == entityID::human && (!((human*)this)->seenCredits))
				{
					human* h = (human*)this;
					h->screen.startCredits = true;
					h->seenCredits = true;
				}
				else
				{
					portalArriveSound->playRandomSound(newDimension, newPosition);
				}
			}
		}
		else
		{
			portalTicks = 0;
		}
	}
	if (stuck)
	{
		// suffocate
		addDamageSource(1, std::make_shared<damageSource>(suffocationDamage));
	}

	bool addFireTicks = false;
	bool doubleDamage = false;
	if (hitboxContains(calculateHitBox(position), { blockID::soul_fire }))
	{
		addFireTicks = true;
		doubleDamage = true;
	}
	else if (hitboxContains(calculateHitBox(position), { blockID::fire }))
	{
		addFireTicks = true;
	}
	if (isUndeadMob(entityType) && (getVisibleSunLightLevel(dimensionIn->getInternalSunLightLevel(floorVector(position + cvec2(relativeHitbox.getCenter().x, relativeHitbox.pos01().y)))) == maxLightLevel) && (fluidArea == 0))
	{
		if (!isHumanoid(entityType) || ((humanoid*)this)->armorSlots->slots[helmetArmorType - bootsArmorType].count == 0)
		{
			addFireTicks = true;
		}
	}

	if (fluidArea > 0)
	{
		if (!isImmuneToFire(entityType))
		{
			if (getFluidArea(calculateHitBox(position), { blockID::lava }) > 0)
			{
				addDamageSource(4, std::make_shared<damageSource>(fireDamage));
				cint initialLavaBurningTickCount = 300;
				if (fireTicks < initialLavaBurningTickCount)
				{
					fireTicks = initialLavaBurningTickCount;
				}
				addFireTicks = true;
			}
		}
		// dont add fire ticks when in water, but get damage from the lava
		if (getFluidArea(calculateHitBox(position), { blockID::water }) > 0)
		{
			if (fireTicks)
			{
				fireTicks = 0;
				if (!addFireTicks)
				{
					extinguishSound->playRandomSound(dimensionIn, position);
				}
			}
			addFireTicks = false;
		}
	}
	if (addFireTicks)
	{
		setOnFire(2);
	}
	if (fireTicks)
	{
		// each second
		if (currentWorld->ticksSinceStart % ticksPerRealLifeSecond == 0)
		{
			addDamageSource(doubleDamage ? 2 : 1, std::make_shared<damageSource>(fireDamage));
		}
		// for if a player respawned
		fireTicks = math::maximum(fireTicks - 1, 0);
	}
	if (immunityFrameCount)
	{
		immunityFrameCount--;
	}
	if (health <= 0)
	{
		onDeath();
	}
	for (auto it = attachedSounds.begin(); it != attachedSounds.end();) {
		AttachedSound* sound = *it;
		auto oldPtr = it++;
		if (--sound->ticksLeft >= 0) {
			sound->update(this);
		}
		else {
			attachedSounds.erase(oldPtr);
			delete sound;
		}
	}
}
void entity::onDeath()
{
	despawn = true;
}
void entity::physics()
{
	bool newOnGround = false;
	cbool& oldOnGround = onGround;
	vec2 positionAfterCollisions = newPosition;
	axisCollided = vect2<bool>();
	if (collisionCheckLevel != collisionTypeID::willNotCollide)
	{
		//cvec2 oldSpeed = speed;

		fp secondsToCalculateLeft = secondsPerTick;
		do
		{
			//we cannot reset newOnGround every time because when we hit the ground, we lose y speed, and so we'll not collide with the ground anymore
			// newOnGround = false;
			// check hitbox
			// relative to speed on collision axis?

			crectangle2 hitboxAfterCollisions = calculateHitBox(positionAfterCollisions);
			// cvec2 distanceTillEndOfTick = speed * secondsToCalculateLeft;
			collisionDataCollection data = newDimension->getRecursiveHitboxCollisionData(hitboxAfterCollisions, speed);
			data.evaluate(hitboxAfterCollisions, speed, secondsToCalculateLeft);

			// get real values
			constexpr fp maxStepHeight = 0.625;

			//step up on for example stairs
			if (collisionCheckLevel == collisionTypeID::willCollideTop && newOnGround)
			{
				size_t index = std::wstring::npos;
				fp firstCollisionTime = INFINITY;

				// check if another hitbox collided
				const fp hitbox00 = positionAfterCollisions.y + relativeHitbox.y;

				for (size_t stepIndex = 0; stepIndex < data.hitboxes.size(); stepIndex++)
				{
					const collisionData& checkData = data.hitboxes[stepIndex];
					if ((checkData.collisionTime > math::fpepsilon) && (checkData.collisionTime < firstCollisionTime))
					{
						cfp collisionTop = checkData.hitboxCollidingWith.y + checkData.hitboxCollidingWith.h;
						if ((checkData.type != collisionTypeID::willNotCollide) && (checkData.collisionTime > 0) && (checkData.collisionTime < secondsToCalculateLeft) && (collisionTop > hitbox00) && (collisionTop <= (hitbox00 + maxStepHeight)))
						{
							// check if covered
							collisionEdgeData edgeToCheck = collisionEdgeData({ cvec2(checkData.hitboxCollidingWith.x, checkData.hitboxCollidingWith.w) });
							edgeToCheck = edgeToCheck.substractCoveringEdges(data.getEdges(collisionTop, directionID::positiveY));

							if (edgeToCheck.edgeInRange(cvec2((speed.x > checkData.speed.x) ? checkData.hitboxCollidingWith.x : ((checkData.hitboxCollidingWith.x + checkData.hitboxCollidingWith.w) - math::fpepsilon), math::fpepsilon)))
							{
								index = stepIndex;
								firstCollisionTime = checkData.collisionTime;
							}
						}
					}
				}
				//cbool allowStepCollisionType[(size_t)collisionTypeID::count]{
				//	false, true, true};
				if (index != std::wstring::npos)
				{
					collisionData stepCollision = data.hitboxes[index];
					// step up block
					//cfp hitboxHeight = stepCollision.hitboxCollidingWith.y + stepCollision.hitboxCollidingWith.h;

					// stepped up position should be positionaftercollisions

					cvec2 steppedUpPosition = cvec2(speed.x < stepCollision.speed.x ? (stepCollision.hitboxCollidingWith.pos1() - relativeHitbox.pos0) : (stepCollision.hitboxCollidingWith.pos01()) - relativeHitbox.pos10()) + cvec2(0, math::fpepsilon);

					crectangle2 stepHitbox = calculateHitBox(steppedUpPosition);

					const collisionTypeID steppedCollisionType = newDimension->getHitboxCollisionType(stepHitbox);

					if (steppedCollisionType != collisionTypeID::willCollide)
					{
						speed.y = stepCollision.speed.y;
						newOnGround = true;
						positionAfterCollisions = steppedUpPosition;
						secondsToCalculateLeft -= stepCollision.collisionTime;
						continue; // don'T change tickpartleft and evaluate the rest of the time at this new elevation
					}
				}
			}
			//handle basic collisions
			cbool allowCollisionType[(size_t)collisionTypeID::count]{
				false,
				false,
				true };
			collisionData firstCollision = data.getFirstCollision(allowCollisionType);

			if (firstCollision.collisionTime < secondsToCalculateLeft)
			{
				axisCollided = firstCollision.axisCollided;
				stuck = firstCollision.stuck;
			}
			else stuck = false;
			//collide with all top faces which aren't covered up by other hitboxes
			if (collisionCheckLevel == collisionTypeID::willCollideTop)
			{
				const std::vector<collisionData> validCollisions = data.getCollisions(collisionTypeID::willCollideTop);
				for (const collisionData& collision : validCollisions)
				{
					if ((speed.y < collision.speed.y) && (collision.collisionTime < firstCollision.collisionTime && collision.collisionTime < secondsToCalculateLeft) &&
						((collision.hitboxCollidingWith.pos0.y + collision.hitboxCollidingWith.h) < positionAfterCollisions.y))
					{
						crectangle2 collisionTopBorder = crectangle2(
							collision.hitboxCollidingWith.x,
							collision.hitboxCollidingWith.y + collision.hitboxCollidingWith.h,
							collision.hitboxCollidingWith.w, 0);
						// check if the border is not covered up by other blocks
						for (const collisionData& coverData : validCollisions)
						{
							crectangle2 coverHitbox = coverData.hitboxCollidingWith;
							/*covers partially:
							coverHitbox.x < collisionTopBorder.x + collisionTopBorder.w &&
								coverHitbox.x + coverHitbox.w > collisionTopBorder.x &&
								coverHitbox.y <= collisionTopBorder.y &&
								coverHitbox.y + coverHitbox.h > collisionTopBorder.y
							*/
							// must cover completely
							if (coverHitbox.x <= collisionTopBorder.x &&
								coverHitbox.x + coverHitbox.w >= collisionTopBorder.x + collisionTopBorder.w &&
								coverHitbox.y <= collisionTopBorder.y &&
								coverHitbox.y + coverHitbox.h > collisionTopBorder.y)
							{
								goto coveredUp;
							}
						}
						firstCollision = collision;
						newOnGround = true;
						axisCollided.y = true;
					}
				coveredUp:;
				}
			}

			if (oldOnGround)
			{
				if (firstCollision.collisionTime < secondsToCalculateLeft)
				{
					cvec2& noCollisionPosition = newPosition + speed * secondsToCalculateLeft;
					// check if walking on a cactus
					// todo: better method than measuring the entire block height while keeping accuracy
					crectangle2& topHitbox = calculateHitBox(vec2(positionAfterCollisions.x, positionAfterCollisions.y));
					crectangle2& bottomHitbox = calculateHitBox(vec2(positionAfterCollisions.x, noCollisionPosition.y));

					cint left = (int)floor(topHitbox.x);
					cint right = (int)floor(topHitbox.x + topHitbox.w);

					cint currentYLevel = (int)floor(topHitbox.y);
					cint newYLevel = (int)floor(bottomHitbox.y);

					cint fromYLevel = math::minimum(currentYLevel, newYLevel);
					cint toYLevel = math::maximum(currentYLevel, newYLevel);

					for (int checkYLevel = fromYLevel; checkYLevel <= toYLevel; checkYLevel++)
					{
						for (int checkX = left; checkX <= right; checkX++)
						{
							cveci2& checkPosition = cveci2(checkX, checkYLevel);

							collisionDataCollection collection = newDimension->getBlockCollisionData(checkPosition);
							collection.evaluate(hitboxAfterCollisions, speed, secondsToCalculateLeft);

							for (size_t hitboxIndex = 0; hitboxIndex < collection.hitboxes.size(); hitboxIndex++)
							{
								const collisionData currentData = collection.hitboxes[hitboxIndex];
								// stuck in a magma_block block will also damage you
								if ((currentData.collisionTime < (firstCollision.collisionTime + math::fpepsilon)) && ((currentData.type == collisionTypeID::willCollide) || (currentData.collisionTime > 0)))
								{
									collisionEdgeData edgeData = collisionEdgeData({ vec2(currentData.hitboxCollidingWith.x, currentData.hitboxCollidingWith.w) });
									edgeData = edgeData.substractCoveringEdges(data.getEdges(currentData.hitboxCollidingWith.pos01().y, directionID::positiveY));

									if (edgeData.edges.size())
									{
										const blockID& blockBottom = newDimension->getBlockID(checkPosition);

										// collides
										if (blockBottom == blockID::cactus)
										{
											addDamageSource(1, std::make_shared<damageSource>(cactusDamage));
										}
										else if ((blockBottom == blockID::magma_block) && (!sneaking))
										{
											addDamageSource(1, std::make_shared<damageSource>(fireDamage));
										}
										else if (isPressurePlate(blockBottom))
										{
											if (isMob(entityType) || (!isStonePressurePlate(blockBottom)))
											{
												((pressurePlateData*)(newDimension->getBlockData(checkPosition)))->addEntityStandingOn(newDimension, checkPosition, this);
											}
										}
										break;
									}
								}
							}
						}
					}
				}
			}
			//corner case
			if (firstCollision.axisCollided.x && firstCollision.axisCollided.y && !firstCollision.stuck) {
				//square instead of ABS
				cvec2& relativeSpeed = math::squared(speed - firstCollision.speed);
				//choose one axis to go with. let's collide on the axis with the least speed. when no axis has the least speed (45 degrees hit on corner), collide on both
				firstCollision.axisCollided.x = relativeSpeed.y > relativeSpeed.x;
				firstCollision.axisCollided.y = relativeSpeed.x > relativeSpeed.y;
			}

			if (firstCollision.axisCollided.y)
			{
				newOnGround = true;
			}

			if (!newOnGround && sneaking && oldOnGround)
			{
				speed = vec2(); // prevent from going off the edge
				positionAfterCollisions = newPosition;
				newOnGround = true;
				// continue, maybe a piston is pushing the sneaking person?
			}
			else if (firstCollision.collisionTime < secondsToCalculateLeft)
			{
				size_t axesImmediatelyCollided = 0;

				if (!oldOnGround && newOnGround)
				{
					onCollisionWithGround(speed.y - firstCollision.speed.y);
				}
				positionAfterCollisions += speed * firstCollision.collisionTime;


				for (size_t axisIndex = 0; axisIndex < 2; axisIndex++)
				{
					if (firstCollision.axisCollided[axisIndex])
					{
						if (firstCollision.collisionTime > 0)
						{
							// to avoid floating point errors
							if (speed[axisIndex] < firstCollision.speed[axisIndex])
							{
								positionAfterCollisions[axisIndex] += math::fpepsilon;
							}
							else if (speed[axisIndex] > firstCollision.speed[axisIndex])
							{
								positionAfterCollisions[axisIndex] -= math::fpepsilon;
							}
						}
						else
						{
							axesImmediatelyCollided++;
						}
						speed[axisIndex] = firstCollision.speed[axisIndex];
					}
				}
				secondsToCalculateLeft -= firstCollision.collisionTime * secondsToCalculateLeft;
				if (axesImmediatelyCollided == 2)
				{
					break;
				}
			}
			else
			{
				positionAfterCollisions += speed * secondsToCalculateLeft;
				break;
			}

			// if ((firstCollision.collisionTime >= secondsToCalculateLeft) || (secondsToCalculateLeft == 0) || (axesImmediatelyCollided == 2))
			//{
			//	break;
			// }
		} while (true);
	}
	else
	{
		positionAfterCollisions = newPosition + speed * secondsPerTick;
	}
	crectangle2 blockCheckHitbox = calculateHitBox(position);
	newPosition = positionAfterCollisions;
	onGround = newOnGround;

	inCobweb = newDimension->blockRangeContains(cveci2((int)floor(blockCheckHitbox.x), (int)floor(blockCheckHitbox.y)),
		cveci2((int)floor(blockCheckHitbox.x + blockCheckHitbox.w), (int)floor(blockCheckHitbox.y + blockCheckHitbox.h)),
		{ blockID::cobweb });

	// check for fluids at the new position, BEFORE friction is applied, so you can jump out of low water streams with ease
	fluidArea = getFluidArea(blockCheckHitbox, std::vector<blockID>(fluidList, fluidList + fluidCount));


	// if (fluidArea > 0)
	//{
	//	cfp& fluidVolume = (fluidArea / blockCheckHitbox.size.volume()) * getVolume();
	//
	//	//fluid gives an upwards force of [volume in water] * 1000
	//	cfp& fluidDisplaceWeight = fluidVolume * blockList[blockID::water]->weightPerCubicMeter;
	//
	//	//add buoyancy
	//
	//
	//	speed = getSpeedAfterFriction(cvect2<vec2>(speed, cvec2(0, gravityForce)), cvec2(getWeight(), fluidDisplaceWeight));
	// }
	speed = applyNaturalForces(speed);
	//const std::vector<vec3>& frictions = getFrictions();
	//
	//for (vec3 fric : frictions)
	//{
	//	speed = math::lerp(cvec2(fric), speed, fric.z); // getSpeedAfterFriction(cvect2<vec2>(speed, cvec2(fric)), cvec2(getWeight(), fric.z));
	//}
}
bool entity::canTeleportTo(cvec2& position) const
{
	cfp maxFallDistance = 0x3;
	crectangle2 hitboxToTest = calculateHitBox(position);
	cvec2 distanceToTest = vec2(0, -maxFallDistance);
	collisionDataCollection data = dimensionIn->getHitboxCollisionData(hitboxToTest, distanceToTest);
	data.evaluate(hitboxToTest, distanceToTest, 1);
	constexpr bool allowCollisionType[(size_t)collisionTypeID::count]{ false, false, true };
	collisionData firstCollision = data.getFirstCollision(allowCollisionType);
	if (firstCollision.type != collisionTypeID::willCollide)
	{
		return false;
	}
	else
	{
		return firstCollision.collisionTime > 0;
	}
}

void entity::teleportRandomly()
{
	cint teleportTries = 0x40;
	cint maxTeleportDistance = 0x20;
	for (int i = 0; i < teleportTries; i++)
	{
		cvec2 teleportPosition = getRandomPointOnEllipse(currentRandom, crectangle2(position, vec2()).expanded(maxTeleportDistance));
		if (canTeleportTo(teleportPosition))
		{
			teleportTo(dimensionIn, teleportPosition, true);
			return;
		}
	}
}

void entity::respawn()
{
	activeEffects = { statusEffect(statusEffectID::resistance, 3 * ticksPerRealLifeSecond, 5) };
	teleportTo(currentWorld->dimensions[(int)currentWorld->worldSpawnDimension], currentWorld->worldSpawnPoint, false);
	health = getMaxHealth();
	fireTicks = 0;
}

entity::~entity()
{
	unRegisterLinkable(identifier);
	if (tasks)
	{
		delete tasks;
	}
}
void entity::render(const gameRenderData& targetData) const
{
	// render fire
	if (fireTicks)
	{
		crectangle2 hitboxRect = calculateHitBox(position);

		renderTexture(hitboxRect, targetData.worldToRenderTargetTransform, true, standardUpFacingBlockDirection, *blockList[(int)blockID::fire]->tex, targetData.renderTarget, true);
	}
}

void entity::onCollisionWithGround(cfp& verticalSpeed)
{
}

fp entity::getFluidArea(crectangle2& box, const std::vector<blockID>& fluids) const
{
	cveci2& pos0 = floorVector(box.pos0);
	crectanglei2& checkRect = crectanglei2(pos0, (floorVector(box.pos1()) - pos0) + 1);

	fp fluidArea = 0;

	for (cveci2& checkPos : checkRect)
	{
		blockID block = dimensionIn->getBlockID(checkPos);
		blockID fluid = block == blockID::kelp ? blockID::water : block;
		if (std::find(fluids.begin(), fluids.end(), fluid) != fluids.end())
		{
			rectangle2 fluidRect;
			if (block == fluid && checkPos.y == checkRect.y)
			{
				fluidLevel level = ((fluidData*)dimensionIn->getBlockData(checkPos))->currentFluidLevel;

				fluidRect = crectangle2(cvec2(checkPos), cvec2(1, level / (fp)maxFluidLevel));
			}
			else
			{
				fluidRect = crectangle2(cvec2(checkPos), cvec2(1));
			}
			fluidArea += crectangle2::getCollisionArea(box, fluidRect);
		}
	}
	return fluidArea;
}

bool entity::hitboxContains(crectangle2& box, const std::vector<blockID>& blockIDArray)
{
	cint fromX = (int)floor(box.x);
	cint fromY = (int)floor(box.y);
	cvec2 topRight = box.pos1();
	cint toX = (int)floor(topRight.x);
	cint toY = (int)floor(topRight.y);
	// check hitbox for water
	veci2 checkPos = cveci2();
	for (checkPos.y = fromY; checkPos.y <= toY; checkPos.y++)
	{
		for (checkPos.x = fromX; checkPos.x <= toX; checkPos.x++)
		{
			blockID block = dimensionIn->getBlockID(checkPos);
			if (std::find(blockIDArray.begin(), blockIDArray.end(), block) != blockIDArray.end())
			{
				return true;
			}
		}
	}
	return false;
}
bool entity::hitboxContainsOnly(crectangle2& box, const std::vector<blockID>& blockIDArray)
{
	cint fromX = (int)floor(box.x);
	cint fromY = (int)floor(box.y);
	cvec2 topRight = box.pos1();
	cint toX = (int)floor(topRight.x);
	cint toY = (int)floor(topRight.y);
	// check hitbox for water
	veci2 checkPos = cveci2();
	for (checkPos.y = fromY; checkPos.y <= toY; checkPos.y++)
	{
		for (checkPos.x = fromX; checkPos.x <= toX; checkPos.x++)
		{
			blockID block = dimensionIn->getBlockID(checkPos);
			if (std::find(blockIDArray.begin(), blockIDArray.end(), block) == blockIDArray.end())
			{
				return false;
			}
		}
	}
	return true;
}

entity* summonEntity(entity* e, tickableBlockContainer* containerIn, cvec2& position)
{
	e->setInitialPosition(containerIn, position);
	e->addToWorld();
	return e;
}
entity* summonEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position)
{
	return summonEntity(createEntity(entityType), containerIn, position);
}
entity* trySummonEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position)
{
	entity* e = createEntity(entityType);
	e->setInitialPosition(containerIn, position);

	if (containerIn->rootDimension->meetsSpawningConditions(e))
	{
		e->addToWorld();
		return e;
	}
	else
	{

		delete e;
		return nullptr;
	}
}
// https://minecraft.gamepedia.com/Damage
bool entity::addDamageSource(cfp& damage, std::shared_ptr<damageSource> source)
{
	if (source.get()->type == damageType::fireDamage)
	{
		if (isImmuneToFire(entityType) || getEffectLevel(statusEffectID::fireResistance))
		{
			return false;
		}
	}

	cfp reducedDamageByEffects = damage * (1 - (getEffectLevel(statusEffectID::resistance) * 0.2));

	// can take damage while in invincibility frames, if it is higher
	// the highest damage source applies.

	cfp& totalHealth = health + absorptionHealth;
	fp oldTotalHealth = totalHealth;

	if (immunityFrameCount)
	{
		if (reducedDamageByEffects <= lastHitDamage)
		{
			return false;
		}
		oldTotalHealth += lastHitDamage;
	}

	lastHitDamage = reducedDamageByEffects;
	immunityFrameCount = immunityTime;

	cfp& newTotalHealth = oldTotalHealth - reducedDamageByEffects;
	absorptionHealth = math::maximum(absorptionHealth - reducedDamageByEffects, (fp)0);
	health = math::minimum(newTotalHealth, health);
	lastDamageSources.push_back(source);
	return true;
}

void entity::applyStatusEffect(const statusEffect& effect)
{
	if (effect.identifier == statusEffectID::poison)
	{
		cint ticks = math::maximum((int)(25 * pow(0.5, effect.potency)), 1);
		if ((currentWorld->ticksSinceStart % ticks) == 0)
		{
			if (health > 1)
			{
				addDamageSource(math::minimum((fp)1, health - 1), std::make_shared<damageSource>(potionDamage));
			}
		}
	}
	else if (effect.identifier == statusEffectID::regeneration)
	{
		cint ticks = math::maximum((int)(50 * pow(0.5, effect.potency)), 1);
		if ((currentWorld->ticksSinceStart % ticks) == 0)
		{
			heal(1);
		}
	}
}

void entity::addStatusEffects(const std::vector<statusEffect>& effectsToAdd)
{
	for (size_t i = 0; i < effectsToAdd.size(); i++)
	{
		const statusEffect& effect = effectsToAdd[i];
		if (effect.identifier == statusEffectID::instantHealth)
		{
			cfp& amount = 2 * pow(2, effect.potency);
			if (isUndeadMob(entityType))
			{
				addDamageSource(amount, std::make_shared<damageSource>(potionDamage));
			}
			else
			{
				heal(amount);
			}
		}
		else if (effect.identifier == statusEffectID::instantDamage)
		{
			cfp& amount = 3 * pow(2, effect.potency);
			if (isUndeadMob(entityType))
			{
				heal(amount);
			}
			else
			{
				addDamageSource(amount, std::make_shared<damageSource>(potionDamage));
			}
		}
		else
		{
			if (effect.identifier == statusEffectID::absorption)
			{
				cfp& newAbsorptionHealth = (fp)4 * effect.potency;
				if (newAbsorptionHealth > absorptionHealth)
				{
					//cfp &difference = newAbsorptionHealth - absorptionHealth;
					absorptionHealth = newAbsorptionHealth;
				}
			}
			activeEffects.push_back(effect);
		}
	}
}

void entity::heal(cfp& health)
{
	this->health = math::minimum(this->health + health, entityDataList[(int)entityType]->maxHealth);
}
rectangle2 entity::calculateHitBox(const cvec2& pos) const
{
	return crectangle2(relativeHitbox.pos0 + pos, relativeHitbox.size);
}

void entity::serializeMembers(nbtSerializer& s)
{
	// writing *this to the stream would be too risky
	// dont write despawn, type and position to the stream

	if (dimensionIn)
	{
		dimensionID newDimensionID = newDimension->identifier;
		if (s.write)
		{
			serializeNBTValue(s, std::wstring(L"new dimension"), newDimensionID);
		}
		else
		{
			if (serializeNBTValue(s, std::wstring(L"new dimension"), newDimensionID))
			{
				newDimension = currentWorld->dimensions[(int)newDimensionID];
			}
		}
	}

	if (s.push<nbtDataTag::tagList>(std::wstring(L"active effects")))
	{
		if (s.write)
		{
			for (size_t i = 0; i < activeEffects.size(); i++)
			{
				if (s.push<nbtDataTag::tagCompound>())
				{
					activeEffects[i].serializeMembers(s);
					s.pop();
				}
			}
		}
		else
		{
			std::vector<nbtData*> serializedEffectList = s.getChildren();
			activeEffects.clear();
			int i = 0;
			for (nbtData* serializedEffect : serializedEffectList)
			{
				if (s.push(serializedEffect))
				{
					statusEffect effect = statusEffect();
					effect.serializeMembers(s);
					activeEffects.push_back(effect);
					s.pop();
				}
				i++;
			}
		}
		s.pop();
	}
	if (tasks && s.push<nbtDataTag::tagCompound>(std::wstring(L"tasks")))
	{
		tasks->serializeMembers(s);
		s.pop();
	}

	serializeNBTValue(s, std::wstring(L"identifier"), identifier);
	serializeNBTValue(s, std::wstring(L"ticks standing in portal"), portalTicks);
	serializeNBTValue(s, std::wstring(L"portal cooldown"), portalCoolDown);
	serializeNBTValue(s, std::wstring(L"new position"), newPosition);
	serializeNBTValue(s, std::wstring(L"speed"), speed);
	serializeNBTValue(s, std::wstring(L"fire ticks"), fireTicks);
	serializeNBTValue(s, std::wstring(L"immunity frame count"), immunityFrameCount);
	serializeNBTValue(s, std::wstring(L"last hit damage"), lastHitDamage);
	serializeNBTValue(s, std::wstring(L"health"), health);
	serializeNBTValue(s, std::wstring(L"fluid area"), fluidArea);
	serializeNBTValue(s, std::wstring(L"in cobweb"), inCobweb);
	serializeNBTValue(s, std::wstring(L"sneaking"), sneaking);
	serializeNBTValue(s, std::wstring(L"onGround"), onGround);
	serializeNBTValue(s, std::wstring(L"collision type"), collisionCheckLevel);
	serializeNBTValue(s, std::wstring(L"relative hitbox"), relativeHitbox);
}

bool entity::compareSelector(const human& sender, const std::wstring& selectorString) const
{
	if (selectorString == std::wstring(L"@e"))
	{
		return true;
	}
	return false;
}

void entity::teleportTo(dimension* newDimension, cvec2& newPosition, cbool& playTeleportSounds, cvec2& speedAfterTeleportation)
{
	this->newDimension = newDimension;
	this->newPosition = newPosition;
	// to avoid collisions
	speed = speedAfterTeleportation;
	if (dimensionIn != newDimension)
	{
		cveci2 newChunkCoordinates = getChunkCoordinates(newPosition);
		// load the chunk moved to
		newDimension->loadChunkIfNotLoaded(newChunkCoordinates, chunkLoadLevel::entityLoaded);
	}
	if (playTeleportSounds)
	{
		teleportFromSound->playRandomSound(dimensionIn, position);
		teleportToSound->playRandomSound(newDimension, newPosition);
	}
}

void entity::renderHitboxes(const gameRenderData& targetData) const
{
	renderBlockRect(calculateHitBox(), targetData, despawn ? colorPalette::red : colorPalette::white);
	cvec2 pos0 = targetData.worldToRenderTargetTransform.multPointMatrix(position);
	cvec2 pos1 = targetData.worldToRenderTargetTransform.multPointMatrix(newPosition);
	fillLine(targetData.renderTarget, pos0, pos1, solidColorBrush(colorPalette::blue));
}

bool entity::setOnFire(cint ticksToBurn)
{
	if (isImmuneToFire(entityType))
	{
		return false;
	}
	else
	{
		fireTicks += ticksToBurn;
		return true;
	}
}

fp entity::getMaxHealth() const
{
	return entityDataList[entityType]->maxHealth;
}

void entity::addToWorld()
{
	registerLinkable(identifier, this);
	// when serializing an entity, uuids will be wasted, but whatever.
	chunk* chunkToAddTo = dimensionIn->loadChunkIfNotLoaded(getChunkCoordinates(position), chunkLoadLevel::worldGenerationLoaded);

	chunkToAddTo->entityList.push_back(this);
	// allowed, the fastlist is only used for moving entities efficiently
	chunkToAddTo->entityList.update();
}

fp entity::getGravityForce() const
{
	return gravityForce;
}

vec2 entity::applyGroundForce(cvec2& speed) const
{
	//-added, * multiplier
	const auto& sign = math::sign(speed.x);
	//get x to zero 
	fp xValue = math::maximum(speed.x * sign * groundFrictionMultiplier - groundFrictionAdder, (fp)0);
	
	return vec2(xValue * sign, speed.y);
}

vec2 entity::applyNaturalForces(cvec2& speed) const
{
	cfp& windArea = calculateHitBox().size.volume();
	cfp& windWeight = blockList[blockID::air]->weightPerCubicMeter * windArea;

	vec2 newSpeed = speed;
	newSpeed.y -= getGravityForce();

	if (collisionCheckLevel == collisionTypeID::willNotCollide)
	{
		newSpeed *= airFrictionMultiplier;
	}
	else
	{

		if (inCobweb)
		{
			newSpeed *= 0.5;
		}

		if (fluidArea > 0)
		{
			newSpeed *= fluidFrictionMultiplier;
		}

		newSpeed = applyGroundForce(newSpeed);
		newSpeed *= airFrictionMultiplier;

		cvec2& windSpeed = dimensionIn->getWindSpeed(calculateHitBox().getCenter());

		cfp& bodyWeight = getWeight();
		if (bodyWeight != INFINITY)
		{
			newSpeed = math::lerp(windSpeed, newSpeed, bodyWeight == 0 ? bodyWeight : (bodyWeight / (windWeight + bodyWeight)));
		}
	}
	return newSpeed;
}

fp entity::getLengthTouchingGround() const
{
	return onGround ? relativeHitbox.size.x : 0;
}

fp entity::getWeight() const
{
	return entityDataList[entityType]->weight;
}

fp entity::getVolume() const
{
	return entityDataList[entityType]->volume;
}

bool fitEntity(entity* e, tickableBlockContainer* containerIn, cvec2& idealPosition)
{
	vec2 resultPosition = vec2();
	if (containerIn->fitExpandingHitbox(e->relativeHitbox, idealPosition, resultPosition)) {
		summonEntity(e, containerIn, resultPosition);
		return true;
	}
	return false;
}
entity* fitEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& idealPosition)
{
	entity* e = createEntity(entityType);
	if (fitEntity(e, containerIn, idealPosition)) {
		return e;
	}
	else {
		delete e;
		return nullptr;
	}
}
int getEntityIDByName(const std::wstring& name)
{
	for (int i = 0; i < (int)entityID::EntityTypeCount; i++)
	{
		if (entityDataList[i]->name == name)
		{
			return i;
		}
	}
	return -1;
}

bool collidesThisTick(const entity& e1, const entity& e2)
{
	vect2<bool> axisCollided = vect2<bool>();
	bool stuck;
	collideTime2d(e1.calculateHitBox(), e2.calculateHitBox(), (e2.speed - e1.speed) * secondsPerTick, axisCollided, stuck);
	return axisCollided.x || axisCollided.y;
}