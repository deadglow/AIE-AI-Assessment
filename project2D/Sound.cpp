#include "Sound.h"

Sound::Sound(Vector2 position, int loudness, float radius, float duration)
{
	this->position = position;
	this->loudness = loudness;
	this->radius = radius;
	this->duration = duration;
}

Sound::~Sound()
{
}
