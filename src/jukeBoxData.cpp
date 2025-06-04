#include "jukeBoxData.h"
#include "musicList.h"
#include "nbt/nbtSerializer.h"
void jukeBoxData::onBlockRemove(tickableBlockContainer* containerIn, cveci2& position)
{
	stopMusic(containerIn, position);
	recordSlot->dropContent(containerIn, cvec2(position) + 0.5);
}

void jukeBoxData::serializeMembers(nbtSerializer& s)
{
	recordSlot->serialize(s, std::wstring(L"record slot"));
	//if (recordSlot->slots[0].count)
	//{
    //    //not microseconds, because the bit length of microseconds may change
	//	int64_t playProgress = 0;
	//	if (s.write)
	//	{
	//		//if (musicPlaying)
	//		//{
	//		//	playProgress = (int64_t)musicPlaying->getPlayingOffset();
	//		//	s.serializeMembers(std::wstring(L"play progress"), playProgress);
	//		//}
	//	}
	//	else
	//	{
	//		//if (s.serializeMembers(std::wstring(L"play progress"), playProgress))
	//		//{
	//		//	//playMusic(containerIn);
	//		//}
//
	//		//todo: continue playing of music
	//	}
	//}
}

jukeBoxData::~jukeBoxData()
{
	//if (musicPlaying.get())
	//{
	//	handler->stopAudio(musicPlaying);
	//}

	delete recordSlot;
}

void jukeBoxData::playMusic(tickableBlockContainer* containerIn, cveci2& position)
{
	musicPlaying = recordsMusic[(int)recordSlot->slots[0].stackItemID - (int)itemID::music_disc]->playRandomSound(containerIn, cvec2(position) + 0.5);
}

void jukeBoxData::stopMusic(tickableBlockContainer* containerIn, cveci2& position)
{
	audioCollection::stopSound(containerIn, StopSoundPacket(musicPlaying));
	musicPlaying = uuid();
}
