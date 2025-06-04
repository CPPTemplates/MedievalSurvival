#include "audio2d.h"

microseconds audio2d::getDuration()
{
	if (!duration) {
		if (source->type == AudioType::sound) {
			duration = source->buffer->getDuration().asMicroseconds();
		}
		else {
			if (!audioLoaded()) {
				loadAudio();
				duration = ((sf::Music*)playingAudio)->getDuration().asMicroseconds();
				unLoadAudio();
			}
		}
	}
	return duration;
}

bool audio2d::audioLoaded() const
{
	return playingAudio;
}

void audio2d::loadAudio()
{
	if (source->type == AudioType::sound) {
		playingAudio = new sf::Sound(*source->buffer);
	}
	else {
		playingAudio = new sf::Music();
		// this way, even on android we can read from an actual file instead of the internal APK storage
		stream = new sfmlInputStream(std::make_shared<std::ifstream>(source->path, getOpenMode(false)));
		if (!((sf::Music*)playingAudio)->openFromStream(*stream)) {
			throw "file not found";
		}
	}

}


sf::SoundSource::Status audio2d::getStatus() const
{
	assumeInRelease((bool)playingAudio);
	return playingAudio->getStatus();
}


void audio2d::unLoadAudio()
{
	if (playingAudio)
	{
		playingAudio->stop();
		delete playingAudio;
		playingAudio = nullptr;
	}
	if (source->type == music) {
		delete stream;
		stream = nullptr;
	}
}


void audio2d::setMinDistance(cfp& minDistance)
{
	audio2d::minDistance = minDistance;
	playingAudio->setMinDistance((float)minDistance);
}


void audio2d::setAttenuation(cfp& attenuation)
{
	audio2d::attenuation = attenuation;
	playingAudio->setAttenuation((float)attenuation);
}


void audio2d::setVolume(cfp& newVolume)
{
	if (audio2d::volume != newVolume)
	{

		audio2d::volume = newVolume;
		updateVolume();
	}
}

void audio2d::setVolumeMultiplier(cfp& mult)
{
	if (audio2d::volumeMultiplier != mult) {
		audio2d::volumeMultiplier = mult;
		updateVolume();
	}
}

void audio2d::updateVolume() const
{
	if (playingAudio) {
		playingAudio->setVolume((float)(audio2d::volume * audio2d::volumeMultiplier));
	}
}

void audio2d::setPitch(cfp& newPitch)
{
	audio2d::pitch = newPitch;
	if (playingAudio)
	{
		playingAudio->setPitch((float)newPitch);
	}
}


void audio2d::setPlayingOffset(const microseconds& offset)
{
	return applyToOriginal([&](auto* audio) {
		return audio->setPlayingOffset(sf::microseconds(offset));
		});
}


microseconds audio2d::getPlayingOffset()
{
	return applyToOriginal([&](auto* audio) {
		return audio->getPlayingOffset().asMicroseconds();
		});
}


void audio2d::setPosition(cvec2& newPosition)
{
	playingAudio->setPosition({ (float)newPosition.x, (float)newPosition.y, 0 });
	pos = newPosition;
}


void audio2d::play()
{
	playingAudio->play();
	if (pitch != 1)
	{
		playingAudio->setPitch((float)pitch);
	}
	updateVolume();
	applyToOriginal([&](auto* audio) {
		audio->setLooping(shouldLoop);
		});
	playingAudio->setSpatializationEnabled(isSpatial);
	if (isSpatial)
	{
		playingAudio->setPosition({ (float)pos.x, (float)pos.y, 0 });
		setVelocity(speed);
	}
}


void audio2d::stop()
{
	playingAudio->stop();
}

void audio2d::setVelocity(cvec2& newSpeed) const
{
	playingAudio->setVelocity({ (float)newSpeed.x, (float)newSpeed.y, 0.f });
}
audio2d::~audio2d()
{
	//the audio has to be deleted first, as it depends on the stream
	delete playingAudio;
	playingAudio = nullptr;
	delete stream;
}