#include "ColliderBox.h"

void ColliderBox::GenerateBox(Vector2 halfExtents)
{
	pointCount = 4;
	this->halfExtents = halfExtents;

	points = new Vector2[4]();

	//Generate four points of a box
	points[0] = -halfExtents;
	points[1] = Vector2(halfExtents.x, -halfExtents.y);
	points[2] = halfExtents;
	points[3] = -points[1];

}

Vector2 ColliderBox::GetHalfExtents()
{
	return halfExtents;
}
