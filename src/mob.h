#pragma once
#include "entity.h"
#include "math/bodypart2d.h"
struct soundCollection;
struct BodyPartSound;
struct mob :public entity
{
	bodyPart2D* mainBodyPart = nullptr;
	bodyPart2D* head = nullptr;

	int ambientSoundCoolDown = 0;

	//abilities
	bool flying = false;
	bool walking = false;

	int jumpStamina = 0;//the amount of ticks before this mob can jump again

	//the total distance the legs covered, NOT the total distance the legs walked
	fp totalLegDistance = 0;

	uuid UUIDRidingOn = uuid();//the entity that this mob rides on
	uuid selectedUUID = uuid();

	//the number of ticks this mob is searching for a partner. negative means that it has bred recently
	int ticksInLove = 0;
	uuid partnerFound = uuid();

	vec2 lookingAt;

	veci2 selectedBlockPosition = veci2();
	veci2 adjacentBlockPosition = veci2();
	vec2 exactBlockIntersection = vec2();
	vec2 exactEntityIntersection = vec2();
	tickableBlockContainer* selectedBlockContainer = nullptr; //will be updated with updateselection, so no need to serialize or something

	bool wantsToSprint = false;
	bool wantsToJump = false;
	bool wantsToSneak = false;
	bool wantsToGoUp = false;
	bool wantsToGoDown = false;
	bool wantsToGoLeft = false;
	bool wantsToGoRight = false;
	bool wantsToHit = false;

	int ticksSinceToolUsed = 0;

	entity* getSelectedEntity();

	virtual int getDefencePoints() const;
	virtual void tick() override;
	BodyPartSound* playSoundAtHead(std::shared_ptr<soundCollection> collection);

	virtual void onDeath() override;
	virtual bool addDamageSource(cfp& damage, std::shared_ptr<damageSource> source) override;

	virtual fp getTotalCoolDownTime() const;

	virtual void updateBodyParts() const;

	virtual fp CalculateFallDamage(cfp& vertspeed);
	vec2 getHeadPosition(cbool& shouldUpdateBodyParts = true) const;
	virtual ~mob();
	mob(const entityID& entityType);
	void exitRodeEntity();
	void renderBodyPart(bodyPart2D* const& b, cmat3x3& transform, const gameRenderData& targetData) const;

	template<typename brush0Type>
	inline void renderBodyPart(bodyPart2D* const& b, cmat3x3& transform, const brush0Type& skin, const gameRenderData& targetData) const;
	virtual void updateSelection();
	void resetAmbientSoundCoolDown();

	virtual fp getGravityForce() const override;
	virtual vec2 getJumpPower() const;
	virtual fp getAttackDamage() const;

	virtual void onCollisionWithGround(cfp& verticalSpeed) override;
	//does not initialize the head
	virtual void serializeValue(nbtSerializer& s) override;
	virtual void render(const gameRenderData& targetData) const override;
	bool shouldJump(bool& wantsToGoLeft, bool& wantsToGoRight) const;
	void lookForward();
	void lookAtEntity(entity* const& e);
	void flipBodyToLookingDirection();
	void flipBodyToWalkingDirection();
	void flipBodyToSpeedDirection();
	virtual bool goToPosition(cvec2& destination);
	//look at 'lookingat'. CALL THIS ONCE YOU UPDATED ALL OTHER BODY PARTS
	void updateHeadAngle()  const;
	fp getArmRange() const;
	void updateBodyPartSize(bodyPart2D* bodyPart, fp multiplier) const;

	void setAge(cint& newAge);
	//use setAge to modify this!
	int age = ticksPerDay;
};
template<typename brush0Type>
inline void mob::renderBodyPart(bodyPart2D* const& b, cmat3x3& transform, const brush0Type& skin, const gameRenderData& targetData) const
{
	if (getEffectLevel(statusEffectID::invisibility) == 0)
	{
		if (immunityFrameCount)
		{
			//took damage, draw red
			const solidColorBrush& redSkinBrush = solidColorBrush(color(color::halfMaxValue, color::maxValue, 0, 0));
			const auto& mixer = colorMixer<solidColorBrush, brush0Type>(redSkinBrush, skin);
			b->renderRecursive(targetData.renderTarget, transform, mixer);
		}
		else
		{
			//just draw the skin
			b->renderRecursive(targetData.renderTarget, transform, skin);
		}
	}
}

