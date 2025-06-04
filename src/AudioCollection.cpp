#include "audioCollection.h"
#include <math/random/random.h>
#include <constants.h>
#include "tickableBlockContainer.h"
#include <server.h>
#include <math/sound/sound.h>
#include "StartSoundPacket.h"
#include "human.h"
#include <folderList.h>
#include <regex>
#include <resourcePack.h>
#include "SoundReference.h"
#include <sound/soundHandler2D.h>
#include <resourceLoader.h>

std::unordered_map<std::wstring, audioCollection*> globalAudioCollectionList = std::unordered_map<std::wstring, audioCollection*>();

uuid audioCollection::playRandomSound(tickableBlockContainer* containerIn, cvec2& position, cfp& volume, cfp& pitch, cbool& shouldLoop)
{
	constexpr int maxSounds = 0x10000;
	//we choose an index higher than the amount of sounds, because it will get modulated by the amount of sounds the client has anyways.
	return playSound(randIndex(currentRandom, maxSounds), containerIn, position, volume, pitch, shouldLoop);
}
uuid audioCollection::playSound(csize_t& index, tickableBlockContainer* containerIn, cvec2& position, cfp& volume, cfp& pitch, cbool& shouldLoop)
{
	const dimension* soundDimension = containerIn->rootDimension;
	cvec2& absolutePosition = containerIn->containerToRootTransform.multPointMatrix(position);
	uuid id = randomUUID(currentRandom);

	std::shared_ptr<StartSoundPacket> packet = std::make_shared<StartSoundPacket>(id, position, key, (int)index, volume, pitch, shouldLoop);

	// get all players from soundDimension
	//check their new position. this is very important, because for example they should hear the portal arrive sound when they go through a portal!
	for (const auto* const& c : currentServer->clients)
	{
		const human* const& p = c->player;
		if (p->newDimension == soundDimension)
		{
			// now check for each player if they can hear it (players have different hearing ranges, for example when zoomed out)
			if ((p->newPosition - absolutePosition).lengthSquared() < math::squared(getHearingRange2D(p->visibleRangeXWalk)))
				p->screen.soundPacketsToSend.push_back(packet);
		}
	}

	return id;
}

void audioCollection::addAudioFile(const stdPath& path) {
	audioToChooseFrom.push_back(std::make_shared<AudioSource>(path, type));
}

void audioCollection::addAudioFileName(const stdPath& path)
{
	key = path.lexically_relative(generalSoundFolder).wstring(); // - generalSoundFolder;

	key = std::regex_replace(key, std::wregex(L"\\/|\\\\"), L".");
	// key.replace(key.begin(), key.end(), stdPath::preferred_separator, L'.');

	globalAudioCollectionList[key] = this;
	bool loaded = false;
	stdPath defaultPath = path;
	defaultPath += L".ogg";

	stdPath foundPath;
	if (getLastResourceLocation(defaultPath, foundPath))
	{
		addAudioFile(foundPath);
		loaded = true;
	}
	// warning: starts with 1
	int i = 1;
	while (true)
	{
		stdPath pathNumber = path;
		pathNumber += std::to_wstring(i) + L".ogg";
		if (getLastResourceLocation(pathNumber, foundPath))
		{
			addAudioFile(foundPath);
			loaded = true;
		}
		else
			break;
		i++;
	}
	if (!loaded)
	{
		handleError(std::wstring(L"no sound found in " + path.wstring()));
	}
}
void audioCollection::sendSoundPacket(tickableBlockContainer* containerIn, const std::shared_ptr<SoundPacket>& packet)
{
	const dimension* soundDimension = containerIn->rootDimension;

	// get all players from soundDimension
	//check their new position. this is very important, because for example they should hear the portal arrive sound when they go through a portal!
	for (const auto* const& c : currentServer->clients)
	{
		const human* const& p = c->player;
		if (p->newDimension == soundDimension)
		{
			//copy
			p->screen.soundPacketsToSend.push_back(packet);
		}
	}
}

void audioCollection::updateSound(tickableBlockContainer* containerIn, const UpdateSoundPacket& packet)
{
	//copy
	sendSoundPacket(containerIn, std::make_shared<UpdateSoundPacket>(packet));
}

void audioCollection::stopSound(tickableBlockContainer* containerIn, const StopSoundPacket& packet)
{
	//copy
	sendSoundPacket(containerIn, std::make_shared<StopSoundPacket>(packet));
}
void audioCollection::playSoundToClient(const StartSoundPacket& packet) const
{
	// make sure that the sound index is in the right range
	// we might have added or removed sounds in the mean time
	// by using %, we make every sound that sounds the same for other clients also sound the same for this client
	cint& modifiedIndex = packet.soundIndex % (int)audioToChooseFrom.size();

	const std::shared_ptr<AudioSource>& source = audioToChooseFrom[modifiedIndex];

	cbool& spatial = packet.position.has_value();
	std::shared_ptr<SoundReference<audio2d>> soundToPlay = std::make_shared<SoundReference<audio2d>>(
		source, spatial ? *packet.position : vec2(), packet.volume, packet.pitch, spatial, packet.shouldLoop);
	soundToPlay->id = packet.id;
	handler.playAudio(soundToPlay);
}

void audioCollection::addCollection(const audioCollection& collection)
{
	audioToChooseFrom.insert(audioToChooseFrom.end(), collection.audioToChooseFrom.begin(), collection.audioToChooseFrom.end());
}

std::shared_ptr<audio2d> audioCollection::playRandomAudioToClient() const
{
	const auto newAudio = std::make_shared<audio2d>(audioToChooseFrom[randIndex(currentRandom, audioToChooseFrom.size())]);
	newAudio->isSpatial = false;
	handler.playAudio(newAudio);
	return newAudio;
}
