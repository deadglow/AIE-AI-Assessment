#pragma once
#include "ColliderPolygon.h"
class ColliderBox :
	public ColliderPolygon
{
public:
	using ColliderPolygon::ColliderPolygon;
	~ColliderBox() {}

	//Creates a polygon with 4 sides with the given half extets
	void GenerateBox(Vector2 halfExtents);
	Vector2 GetHalfExtents();

protected:
	Vector2 halfExtents;
};

