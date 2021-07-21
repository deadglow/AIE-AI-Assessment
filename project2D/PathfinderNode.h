#pragma once
#include "Vector2.h"

using namespace MathMan;

#define NEIGHBOUR_COUNT 8

struct PathfinderNode
{
	int x = 0;
	int y = 0;
	int gScore = 0;
	PathfinderNode* previous = nullptr;

	bool blocked = false;
	int cost = 0;

	PathfinderNode* neighbours[8];
};