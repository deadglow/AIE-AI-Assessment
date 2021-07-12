#include "ColliderPolygon.h"
#include <cmath>
#include "Entity.h"

ColliderPolygon::ColliderPolygon(Entity* ent) : Collider::Collider(ent)
{

}

ColliderPolygon::~ColliderPolygon()
{
	delete[] points;
}

int ColliderPolygon::GetPointCount()
{
	
	return pointCount;
}

int ColliderPolygon::GetTransformedPoints(Vector2*& outArray)
{
	outArray = new Vector2[pointCount]();

	for (int i = 0; i < pointCount; ++i)
	{
		outArray[i] = entity->GetTransform()->GetGlobalTransform() * points[i];
	}
	
	return pointCount;
}

void ColliderPolygon::GenerateShape(int sides, float radius)
{
	if (sides < 3)
		throw "A polygon must be 3 or more sides.";
	
	points = new Vector2[sides];

	float rotationGap = 2 * (float)PI / sides;

	for (int i = 0; i < sides; ++i)
	{
		points[i] = Vector2(cosf(rotationGap * i), sinf(rotationGap * i)) * radius;
	}
}

void ColliderPolygon::AssignPoints(Vector2* pointArray, int pointCount)
{
	this->points = pointArray;
	this->pointCount = pointCount;
}

ColliderType ColliderPolygon::GetColliderType()
{
	return ColliderType::Polygon;
}