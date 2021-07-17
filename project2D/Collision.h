#pragma once
#include "Vector2.h"

using namespace MathMan;

class Collider;

struct Collision
{
	Collider* other;
	Vector2 resolveVector;
	float resolveDistance;
	Vector2 relativePosition;
	float finalRestitution;
	Vector2 finalMomentum;
};