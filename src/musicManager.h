#pragma once
#include "audioCollection.h"
#include <sound/audio2d.h>
#include <memory>
void updateMusic(const audioCollection* const& musicIfFinished);
void stopMusic();

void replaceMusic(const audioCollection* const& newMusic);
void playMusic(const audioCollection* const& newMusic);

extern std::shared_ptr<audio2d> currentlyPlayingMusic;
extern const audioCollection* currentlyPlayingCollection;