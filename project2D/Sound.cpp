#include "Sound.h"

Sound::Sound(Vector2 position, int loudness, float radius)
{
	this->position = position;
	this->loudness = loudness;
	this->radius = radius;
}

Sound::~Sound()
{
}
