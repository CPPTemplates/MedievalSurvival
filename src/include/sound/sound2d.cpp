#include "sound2d.h"
#include <SFML/Audio/SoundBuffer.hpp>
void sound2d::loadAudio() {
    playingAudio = new sf::Sound(*buffer);
}

microseconds sound2d::getDuration() {
    if (!buffer) {
        throw "buffer not found";
    }
    return buffer->getDuration().asMicroseconds();
}