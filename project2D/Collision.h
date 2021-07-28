#pragma once
#include "Vector2.h"

using namespace MathMan;

class Collider;

struct Collision
{
	//Most of these aren't actually used but no time to restructure the collison stuff
	Collider* other;
	Vector2 resolveVector;
	float resolveDistance;
	Vector2 relativePosition;
	float finalRestitution;
	Vector2 finalMomentum;
};