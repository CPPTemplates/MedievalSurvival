#include "AudioType.h"
#include <filesystem/filemanager.h>
#pragma once
struct AudioSource {
	AudioType type;
	stdPath path;
	sf::SoundBuffer* buffer = nullptr;
	AudioSource(stdPath path, AudioType type);
	~AudioSource() {
		//works too when buffer is null
		delete buffer;
	}
};