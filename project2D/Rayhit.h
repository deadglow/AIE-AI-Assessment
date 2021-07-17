#pragma once

#include "Vector2.h"

using namespace MathMan;

class Collider;

struct RayHit
{
	Vector2 point;
	float distance;
	Collider* hitCollider;
};