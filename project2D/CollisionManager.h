#pragma once
#include <list>
#include "Collision.h"
#include "Collider.h"
#include "ColliderPolygon.h"
#include "ColliderCircle.h"
#include "Rayhit.h"

class CollisionManager
{
public:

	CollisionManager() {};
	~CollisionManager() {};

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);

	void CheckCollisions();

	//Test two polygons
	bool TestPolygons(ColliderPolygon* a, ColliderPolygon* b, Vector2& resolveVector, float& resolveDistance, bool flipResults);
	bool TestCircles(ColliderCircle* a, ColliderCircle* b, Vector2& resolveVector, float& resolveDistance, bool flipResults);
	bool TestPolygonToCircle(ColliderPolygon* poly, ColliderCircle* circ, Vector2& resolveVector, float& resolveDistance, bool flipResults);
	bool RayCast(Vector2 origin, Vector2 direction, float maxDistance, RayHit& rayHit);
	bool RayCastToPolygon(ColliderPolygon* collider, Vector2 origin, Vector2 direction, float maxDistance, RayHit& rayHit);

	void DrawColliders(aie::Renderer2D* renderer);
protected:
	//Project points from vertices onto axis and return the lowest (x) and highest(y) value
	Vector2 ProjectVertsMinMax(Vector2 axis, Vector2* verts, int vertCount);
	
	std::list<Collider*> colliders;
};