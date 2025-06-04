#include "AudioSource.h"
#include <filesystem/sfmlInputStream.h>
#include <filesystem/fileio.h>

AudioSource::AudioSource(stdPath path, AudioType type) : path(path), type(type) {
	if (type == AudioType::sound) {

		buffer = new sf::SoundBuffer();
		// this way, even on android we can read from an actual file instead of the internal APK storage
		auto stream = sfmlInputStream(std::make_shared<std::ifstream>(path, getOpenMode(false)));

		// sf::InputStream str
		buffer->loadFromStream(stream);
	}

}
