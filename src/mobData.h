#pragma once
#include "math/wave/waveShaper.h"
#include "experienceDrop.h"
#include "lootTable.h"
#include "audioCollection.h"
#include "math/graphics/resolutiontexture.h"
#include "entityData.h"

struct mobData : public entityData
{
	mobData(const entityData& data, resolutionTexture* skin, std::shared_ptr<audioCollection> walkSound, std::shared_ptr<audioCollection> ambientSound, std::shared_ptr<audioCollection> hurtSound, std::shared_ptr<audioCollection> deathSound, cfp& walkingSpeed, const waveShaper& legSwingSynchronizer, const color& spawnEggColor, const color& spawnEggOverlayColor, cfp& attackSpeed = 1.0, cfp& attackDamage = 1.0, experienceDrop* experienceWhenKilled = new experienceDrop(0, 0), cfp& flyingSpeed = 0);
	fp walkingSpeed = 0;
	fp flyingSpeed = 0;
	fp attackSpeed = 0;
	//at NORMAL difficulty
	fp attackDamage = 0;

	//appearance
	std::shared_ptr<audioCollection> ambientSound = std::shared_ptr<audioCollection>();
	std::shared_ptr<audioCollection> hurtSound = std::shared_ptr<audioCollection>();
	std::shared_ptr<audioCollection> deathSound = std::shared_ptr<audioCollection>();
	std::shared_ptr<audioCollection> stepSound = std::shared_ptr<audioCollection>();

	resolutionTexture* skin;
	waveShaper legSwingSynchronizer;

	color spawnEggColor = color();
	color spawnEggOverlayColor = color();


	//loot
	std::shared_ptr <lootTable> dropsWhenKilled = std::make_shared<lootTable>();
	experienceDrop* experienceWhenKilled = nullptr;


	virtual ~mobData() override;
	virtual fp getSpawnWeight(dimension* dimensionIn, cveci2& position);
};
mobData* getMobData(const entityID& entityType);