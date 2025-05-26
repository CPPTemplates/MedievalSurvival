#include "nbt/nbtSerializable.h"
#include "interface/idestructable.h"
#include "entityID.h"
#include "collisionTypeID.h"
#include "damageSource.h"
#include "statusEffect.h"
#include "gameRenderData.h"
#include "itemID.h"
#include "math/collisions.h"
#include <list>
struct AttachedSound;
struct dimension;

struct entityAI;
struct decisions;
constexpr fp airFrictionMultiplier = 0.95;
constexpr fp airTerminalVelocityMultiplier = getTerminalVelocityMultiplier(airFrictionMultiplier);
constexpr fp groundFrictionMultiplier = 0.9;
//force has to pull at least this amount of blocks / second to get something moving
constexpr fp groundFrictionSpeedReductionPerSecond = 0.5f;
constexpr fp groundFrictionAdder = groundFrictionSpeedReductionPerSecond * secondsPerTick;
//this will get the human walking speed perfectly.
constexpr fp groundTerminalVelocityMultiplier = getTerminalVelocityAdder(groundFrictionMultiplier, groundFrictionAdder, humanWalkingSpeed) / humanWalkingSpeed;
constexpr fp fluidFrictionMultiplier = 0.8;
constexpr fp ladderFrictionMultiplier = 0.5;

#pragma once
struct entity :IDestructable, nbtSerializable
{
	//the neuralNetwork for this entity
	entityAI* tasks = nullptr;
	decisions* tasksToExecute = nullptr;

	entityID entityType = (entityID)0;
	uuid identifier;
	dimension* dimensionIn = nullptr;
	//the current position of a mob. use this in tick() functions of other mobs and entities to prevent any advantage
	vec2 position = vec2();
	vec2 speed = vec2();//represented in blocks per real life second (per 20 ticks)
	int portalTicks = 0;
	int portalCoolDown = 0;

	vec2 newPosition = vec2();
	dimension* newDimension = nullptr;

	int fireTicks = 0;//the amount of ticks this entity remains on fire
	int immunityFrameCount = 0;//the amount of frames this mob still has invincibility from any lower amounts of damage
	fp lastHitDamage = 0;//the health level before the mob took damage on the last hit.
	fp health = 0, absorptionHealth = 0;
	bool despawn = false;

	fp fluidArea = 0;//the area of the entity's hitbox that is touching fluid in blocks ^ 2
	bool inCobweb = false;
	bool sneaking = false;
	bool onGround = false;
	//wether the entity is stuck in blocks, making it unable to move and take suffocation damage.
	bool stuck = false;

	vect2<bool> axisCollided = vect2<bool>();

	collisionTypeID collisionCheckLevel = collisionTypeID::willCollideTop;
	rectangle2 relativeHitbox = rectangle2();//the hitbox relative to its position (if it stood at 0,0)

	//remembers damage sources for 5 seconds
	std::list<std::shared_ptr<damageSource>> lastDamageSources;
	std::list<AttachedSound*> attachedSounds;

	std::vector<statusEffect> activeEffects;

	int getEffectLevel(const statusEffectID& id) const;

	vec2 getRenderOffset(const gameRenderData& targetData) const;

	void setInitialPosition(tickableBlockContainer* containerIn, cvec2& position);
	void addSpeed(cvec2& additionalSpeed);

	vec2 handleCollision(cvec2& otherSpeed, cfp& otherMass);

	entity(const entityID& entityType);
	virtual void tick();
	virtual bool addDamageSource(cfp& damage, std::shared_ptr<damageSource> source);
	virtual void applyStatusEffect(const statusEffect& effect);
	virtual void addStatusEffects(const std::vector<statusEffect>& effectsToAdd);

	void heal(cfp& health);
	//identifier has to be added here, because from this point on other entities will be comparing UUIDS to differentiate entities
	void addToWorld();

	virtual void onDeath();
	void physics();
	virtual void render(const gameRenderData& targetData) const;
	virtual void onCollisionWithGround(cfp& verticalSpeed);

	fp getFluidArea(crectangle2& box, const std::vector<blockID>& fluids) const;
	bool hitboxContains(crectangle2& box, const std::vector<blockID>& blockIDArray);
	bool hitboxContainsOnly(crectangle2& box, const std::vector<blockID>& blockIDArray);

	rectangle2 calculateHitBox() const;
	virtual rectangle2 calculateHitBox(const cvec2& pos) const;
	virtual void serializeValue(nbtSerializer& s) override;
	virtual bool compareSelector(const human& sender, const std::wstring& selectorString) const;
	void teleportTo(dimension* newDimension, cvec2& newPosition, cbool& playTeleportSounds, cvec2& speedAfterTeleportation = cvec2());
	virtual void renderHitboxes(const gameRenderData& targetData) const;
	bool setOnFire(cint ticksToBurn);

	virtual fp getMaxHealth() const;

	virtual fp getGravityForce() const;
	virtual vec2 applyGroundForce(cvec2& speed) const;
	/// <summary>
	/// applies natural forces to the current speed of the player
	/// </summary>
	/// <returns>the modified speed when natural forces are applied</returns>
	virtual vec2 applyNaturalForces(cvec2& speed) const;

	virtual fp getLengthTouchingGround() const;

	virtual fp getWeight() const;
	virtual fp getVolume() const;

	virtual bool canTeleportTo(cvec2& position) const;
	void teleportRandomly();

	//virtual void setAsPlayableCharachter();
	virtual void respawn();
	~entity();
};

entity* createEntity(const entityID& entityType);
entity* createEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position);
entity* summonEntity(entity* e, tickableBlockContainer* containerIn, cvec2& position);
entity* summonEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position);
entity* trySummonEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position);
//fit this entity at this position. we guarantee that its hitbox will contain the position
bool fitEntity(entity* e, tickableBlockContainer* containerIn, cvec2& idealPosition);
entity* fitEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& idealPosition);
int getEntityIDByName(const std::wstring& name);
bool collidesThisTick(const entity& e1, const entity& e2);