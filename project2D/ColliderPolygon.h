#pragma once
#include "Collider.h"
class ColliderPolygon :
	public Collider
{
public:
	ColliderPolygon(Entity* ent);
	virtual ~ColliderPolygon();

	virtual ColliderPolygon* CloneTo(Entity* ent) override;

	int GetPointCount();

	//Creates memory! Outputs array of points transformed by this object's transform
	int GetTransformedPoints(Vector2*& outArray);

	void GenerateShape(int sides, float radius);

	void AssignPoints(Vector2* pointsArray, int pointCount);

	ColliderType GetColliderType() override;

	void Draw(aie::Renderer2D* renderer) override;

protected:
	Vector2* points;
	int pointCount;
};

