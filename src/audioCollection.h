#pragma once
#include "interface/idestructable.h"
#include <string>
#include "filesystem/filemanager.h"
#include "math/uuid.h"
#include <UpdateSoundPacket.h>
#include "include/sound/AudioType.h"
struct tickableBlockContainer;
#include <StopSoundPacket.h>
#include "include/sound/AudioSource.h"
#include <sound/audio2d.h>
#include "SoundReference.h"
struct StartSoundPacket;
struct audioCollection :IDestructable
{
	AudioType type;
	std::vector<std::shared_ptr<AudioSource>> audioToChooseFrom = std::vector<std::shared_ptr<AudioSource>>();
	std::wstring key;
	inline audioCollection(const stdPath& path, const AudioType& type): type(type) {
		addAudioFileName(path);
	}
	void addAudioFileName(const stdPath& path);
	void addAudioFile(const stdPath& path);
	uuid playRandomSound(tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1, cbool& shouldLoop = false);
	uuid playSound(csize_t& index, tickableBlockContainer* containerIn, cvec2& position, cfp& volume = 1, cfp& pitch = 1, cbool& shouldLoop = false);
	static void sendSoundPacket(tickableBlockContainer* containerIn, const std::shared_ptr<SoundPacket>& packet);
	static void updateSound(tickableBlockContainer* containerIn, const UpdateSoundPacket& packet);
	static void stopSound(tickableBlockContainer* containerIn, const StopSoundPacket& packet);
	void playSoundToClient(const StartSoundPacket& packet) const;
	void addCollection(const audioCollection& collection);
	std::shared_ptr<audio2d> playRandomAudioToClient() const;
};
extern std::unordered_map<std::wstring, audioCollection*> globalAudioCollectionList;