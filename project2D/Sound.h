#pragma once
#include "Vector2.h"

using namespace MathMan;

struct Sound
{
	Sound() {};
	Sound(Vector2 position, int loudness, float radius);
	~Sound();
	
	Vector2 position = Vector2();
	int loudness = 1;
	float radius = 1.0f;
};

