#include "CollisionManager.h"
#include "Entity.h"
#include "Transform.h"

void CollisionManager::AddCollider(Collider* collider)
{
	colliders.push_back(collider);
}

void CollisionManager::RemoveCollider(Collider* collider)
{
	colliders.remove(collider);
}

void CollisionManager::CheckCollisions()
{
	auto iterator = colliders.begin();
	
	while (iterator != colliders.end())
	{
		auto subIterator = iterator;
		subIterator++;
		while (subIterator != colliders.end())
		{
			Collider* a = *iterator;
			Collider* b = *subIterator;

			//Static vs static is unecessary
			if (a->IsStatic() && b->IsStatic())
				continue;

			//Implement collision masks here
			
			float resolveDistance = 0;
			Vector2 resolveVector = Vector2();
			bool collided = false;

			//Check for collisions and assign to resolveDistance
			//


			subIterator++;
		}

		iterator++;
	}
}

Vector2 CollisionManager::ProjectVertsMinMax(Vector2 axis, Vector2* verts, int vertCount)
{
	Vector2 minMax = Vector2();
	minMax.x = Vector2::Dot(axis, verts[0]);
	minMax.y = minMax.x;

	float dot;
	for (int i = 1; i < vertCount; ++i)
	{
		dot = Vector2::Dot(axis, verts[i]);
		if (dot < minMax.x)
			minMax.x = dot;
		if (dot > minMax.y)
			minMax.y = dot;
	}

	return minMax;
}

bool CollisionManager::TestPolygons(ColliderPolygon* a, ColliderPolygon* b, Vector2& resolveVector, float& resolveDistance, bool flipResults)
{
	resolveVector = Vector2();
	resolveDistance = 0.0f;
	float shortestDistance = FLT_MAX;

	//Transformed verts
	Vector2* aVerts;
	Vector2* bVerts;
	int aVertCount, bVertCount;

	aVertCount = a->GetTransformedPoints(aVerts);
	bVertCount = b->GetTransformedPoints(bVerts);

	//Get b - a
	Vector2 abOffset = a->GetEntity()->GetTransform()->GetGlobalPosition() - b->GetEntity()->GetTransform()->GetGlobalPosition();

	//Cache so they don't keep geting created
	Vector2 axis, aMinMax, bMinMax;
	float scalarOffset, minDist, minDistAbs;

	//Loop through all axes, eventually returning to the smallest needed resolve value, and which vector is needed to reach that resolve
	for (int i = 0; i < aVertCount; ++i)
	{
		//Get normal of the side generated from current point to next point
		axis = (aVerts[(i + 1) & aVertCount] - aVerts[i]).RightAngle().Normalised();

		//Gets the minimum and maximum values of the points of each object being projected on the axis
		//x min, y max
		aMinMax = this->ProjectVertsMinMax(axis, aVerts, aVertCount);
		bMinMax = this->ProjectVertsMinMax(axis, bVerts, bVertCount);

		//Offsets minMax by their relative translation on the axis
		scalarOffset = Vector2::Dot(axis, abOffset);
		aMinMax.x += scalarOffset;
		aMinMax.y += scalarOffset;

		//No collision detected since a gap was detected
		if (aMinMax.x - bMinMax.y > 0 || bMinMax.x - aMinMax.y > 0)
			return false;

		//Try swap these some time
		minDist = -(aMinMax.y - bMinMax.x);
		if (flipResults)
			minDist = -minDist;

		minDistAbs = abs(minDist);

		if (minDistAbs < shortestDistance)
		{
			shortestDistance = minDistAbs;

			resolveDistance = minDist;
			resolveVector = axis;
		}

		resolveVector *= resolveDistance;

		return true;
	}
}