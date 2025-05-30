#pragma once
#include "blockData.h"
#include "rectangularSlotContainer.h"

struct jukeBoxData : blockData
{
	virtual bool tick(tickableBlockContainer* containerIn, cveci2& position) override;

	rectangularSlotContainer* recordSlot = nullptr;
	uuid musicPlaying = uuid();

	jukeBoxData() : recordSlot(new rectangularSlotContainer(cveci2(1))) {}
	virtual void onBlockRemove(tickableBlockContainer* containerIn, cveci2& position) override;
	virtual void serializeMembers(nbtSerializer& s) override;
	virtual ~jukeBoxData() override;
	void playMusic(tickableBlockContainer* containerIn, cveci2& position);
};