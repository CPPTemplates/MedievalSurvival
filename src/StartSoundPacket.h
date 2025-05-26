#include "soundPacket.h"
#pragma once
struct StartSoundPacket : SoundPacket {
	std::optional<vec2> position;
	std::wstring key;
	int soundIndex;
	fp volume;
	fp pitch;
	bool shouldLoop;
	StartSoundPacket(const uuid& id, std::optional<vec2> position = std::nullopt, const std::wstring& key = std::wstring(), cint& soundIndex = 0, cfp& volume = 1, cfp& pitch = 1, bool shouldLoop = false) :
		position(position), key(key), soundIndex(soundIndex), volume(volume), pitch(pitch), shouldLoop(shouldLoop), SoundPacket(id, SoundPacketType::start) {
	}
	bool serialize(nbtSerializer& s) override;
};