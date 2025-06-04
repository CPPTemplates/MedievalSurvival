#pragma once
#include "particleID.h"
enum class entityID :int
{
	//first come all the mobs
	//humanoids
	human,
	skeleton,
	stray,
	bogged,
	husk,
	drowned,
	zombie_villager,
	zombie,
	zombiefied_piglin,
	villager,
	enderman, 
	HumanoidCount,
	//slime like

	slime = HumanoidCount,
	magma_cube,

	creeper,

	wolf,

	//passive
	pig,
	cow,
	sheep,
	chicken,

	cod,

	shulker,

	//flying
	blaze,
	ghast,
	//boss
	ender_dragon, 
	MobTypeCount,

	boat = MobTypeCount,
	minecart,

	item,
	experience_orb,
	particle,

	snowball = (int)particle + (int)particleID::count,
	egg,
	ender_pearl,
	ender_eye,
	fishing_bobber,
	arrow,

	ghast_fireball,
	dragon_fireball,
	small_Fireball,

	tnt,
	end_crystal,

	falling_block,
	piston,

	firework_rocket,
	pollen,

	EntityTypeCount, count = EntityTypeCount,
};

constexpr int mobTypeCount = (int)entityID::MobTypeCount;
constexpr int humanoidTypeCount = (int)entityID::HumanoidCount;