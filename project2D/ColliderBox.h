#pragma once
#include "ColliderPolygon.h"
class ColliderBox :
	public ColliderPolygon
{
public:
	using ColliderPolygon::ColliderPolygon;

	void GenerateBox(Vector2 halfExtents);
	Vector2 GetHalfExtents();

protected:
	Vector2 halfExtents;
};

