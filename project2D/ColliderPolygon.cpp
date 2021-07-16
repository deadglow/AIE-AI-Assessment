#include "ColliderPolygon.h"
#include <cmath>
#include "Entity.h"
#include <iostream>

ColliderPolygon::ColliderPolygon(Entity* ent) : Collider::Collider(ent)
{
	pointCount = 0;
}

ColliderPolygon::~ColliderPolygon()
{
	delete[] points;
}

ColliderPolygon* ColliderPolygon::CloneTo(Entity* ent)
{
	ColliderPolygon* col = ent->AddComponent<ColliderPolygon>();
	col->isStatic = this->isStatic;
	col->restitution = this->restitution;
	col->pointCount = this->pointCount;
	col->points = new Vector2[pointCount];
	memcpy_s(col->points, pointCount * sizeof(Vector2), this->points, pointCount * sizeof(Vector2));
	//No need to assign attachedPhysObject since constructor does that

	return col;
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

Collider::ColliderType ColliderPolygon::GetColliderType()
{
	return Collider::ColliderType::Polygon;
}

void ColliderPolygon::Draw(aie::Renderer2D* renderer)
{
	Vector2* points;
	int pointCount = GetTransformedPoints(points);

	Vector2 pos = entity->GetTransform()->GetGlobalPosition();

	Vector2 pointA;
	Vector2 pointB;
	for (int i = 0; i < pointCount; ++i)
	{
		pointA = points[i] + pos;
		pointB = points[(i + 1) % pointCount] + pos;
		renderer->DrawLine(pointA.x, pointA.y, pointB.x, pointB.y, 1.0f, -1.0f);
	}

	delete[] points;
}
