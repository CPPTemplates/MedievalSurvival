#include "idAnalysis.h"
#include "noteBlockData.h"
#include "tickableBlockContainer.h"
#include "soundList.h"
#include "math/sound/sound.h"
#include "nbt/nbtSerializer.h"
bool noteBlockData::tick(tickableBlockContainer* containerIn, cveci2& position)
{
	//make a copy instead of referencing
	const powerLevel lastTickPowerLevel = lastPowerLevel;
	lastPowerLevelData::tick(containerIn, position);

	if (lastPowerLevel > lastTickPowerLevel)
	{
		playNote(containerIn, position);
	}
	return false;
}

void noteBlockData::serializeMembers(nbtSerializer& s)
{
	lastPowerLevelData::serializeMembers(s);
	serializeNBTValue(s, std::wstring(L"note"), note);
}

void noteBlockData::playNote(tickableBlockContainer* containerIn, cveci2& position)
{
	const blockID& blockBelow = containerIn->getBlockID(position + cveci2(0, -1));

	const noteTypeID& idToPlay = getNote(blockBelow);

	noteSounds[(int)idToPlay]->playRandomSound(containerIn, cvec2(position) + 0.5, 1, getNotePitch(note - octaveNoteCount));
}