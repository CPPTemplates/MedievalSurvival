#include "zombifiedPiglin.h"
#include "enderDragon.h"
#include "zombie.h"
#include "skeleton.h"
#include "fluidList.h"
#include "entityData.h"
#include "idList.h"
#include "magmaCube.h"
#include "blaze.h"
#include "ghast.h"
#include "experienceOrb.h"
#include "sheep.h"
#include "creeper.h"
#include "pig.h"
#include "cow.h"
#include "wolf.h"
#include "cod.h"
#include "chicken.h"
#include "shulker.h"
#include "enderman.h"
#include "smallFireBall.h"
#include "ghastFireBall.h"
#include "dragonFireBall.h"
#include "boat.h"
#include "floatingSlot.h"
#include "egg.h"
#include "snowBall.h"
#include "enderPearl.h"
#include "eyeOfEnder.h"
#include "fishingHook.h"
#include "arrow.h"
#include "tnt.h"
#include "endCrystal.h"
#include "fallingBlock.h"
#include "minecart.h"
#include "particle.h"
#include "iconParticle.h"
#include "textureParticle.h"
#include "piston.h"
#include "fireworkRocket.h"
#include "pollen.h"
#include "slime.h"
#include "effectParticle.h"
#include "idAnalysis.h"
#include "world.h"
#include "blockParticle.h"
#include "itemParticle.h"

entity* createEntity(const entityID& entityType)
{
	switch (entityType)
	{
	case entityID::zombiefied_piglin:
		return new zombifiedPiglin();
	case entityID::enderman:
		return new enderman();
	case entityID::slime:
		return new slime();
	case entityID::magma_cube:
		return new magmaCube();
	case entityID::creeper:
		return new creeper();
	case entityID::pig:
		return new pig();
	case entityID::cow:
		return new cow();
	case entityID::sheep:
		return new sheep();
	case entityID::wolf:
		return new wolf();
	case entityID::cod:
		return new cod();
	case entityID::boat:
		return new boat();
	case entityID::experience_orb:
		return new experienceOrb();
	case entityID::item:
		return new floatingSlot();
	case entityID::egg:
		return new egg();
	case entityID::snowball:
		return new snowBall();
	case entityID::ender_pearl:
		return new enderPearl();
	case entityID::ender_eye:
		return new eyeOfEnder();
	case entityID::fishing_bobber:
		return new fishingHook();
	case entityID::arrow:
		return new arrow();
	case entityID::ghast_fireball:
		return new ghastFireBall();
	case entityID::dragon_fireball:
		return new dragonFireBall();
	case entityID::small_Fireball:
		return new smallFireBall();
	case entityID::tnt:
		return new tnt();
	case entityID::end_crystal:
		return new endCrystal();
	case entityID::falling_block:
		return new fallingBlock();
	case entityID::shulker:
		return new shulker();
	case entityID::blaze:
		return new blaze();
	case entityID::ghast:
		return new ghast();
	case entityID::ender_dragon:
		return new enderDragon();
	case entityID::chicken:
		return new chicken();
	case entityID::minecart:
		return new minecart();
	case entityID::piston:
		return new piston();
	case entityID::firework_rocket:
		return new fireworkRocket();
	case entityID::pollen:
		return new pollen();
	default:
		if (isZombie(entityType))
		{
			return new zombie(entityType);
		}
		else if (isSkeleton(entityType))
		{
			return new skeleton(entityType);
		}
		else if (isParticle(entityType)) {
			particleID particleType = (particleID)((int)entityType - (int)entityID::particle);
			switch (particleType)
			{
			case particleID::block:
				return new blockParticle(particleID::block);
			case particleID::item:
				return new itemParticle(particleID::item);
			case particleID::effect:
				return new effectParticle();
			default:
				break;
			}
			if (isIconParticle(particleType)) {
				return new iconParticle(particleType);
			}
			else {
				return new textureParticle(particleType);
			}
		}
		else
		{
			currentWorld->currentChat.addLine(std::wstring(L"summoning of this entity is not implemented"));
			return nullptr;
		}
		break;
	}
}

entity* createEntity(const entityID& entityType, tickableBlockContainer* containerIn, cvec2& position)
{
	entity* e = createEntity(entityType);
	e->setInitialPosition(containerIn, position);
	return e;
}

