#pragma once
#include "Vector2.h"

using namespace MathMan;

struct SoundFieldNode
{
	//Direction to previous node
	Vector2 direction = Vector2();
	int gScore = 0;
};