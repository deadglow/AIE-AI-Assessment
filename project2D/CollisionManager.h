#pragma once
#include <list>
#include "Collision.h"
#include "Collider.h"
#include "ColliderPolygon.h"
#include "ColliderCircle.h"

class CollisionManager
{
public:
	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

	void CheckCollisions();

	//Test two polygons
	bool TestPolygons(ColliderPolygon* a, ColliderPolygon* b, Vector2& resolveVector, float& resolveDistance, bool flipResults = true);
	bool TestCircles(ColliderCircle* a, ColliderCircle* b, Vector2& resolveVector, float& resolveDistance, bool flipResults = true);
protected:
	//Project points from vertices onto axis and return the lowest (x) and highest(y) value
	Vector2 ProjectVertsMinMax(Vector2 axis, Vector2* verts, int vertCount);
	
	std::list<Collider*> colliders;

};