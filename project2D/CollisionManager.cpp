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
		//Create iterator starting at next element from current iterator
		auto subIterator = iterator;
		subIterator++;
		while (subIterator != colliders.end())
		{
			//Dereference iterators to get colliders
			Collider* a = *iterator;
			Collider* b = *subIterator;

			//Static vs static is unecessary
			if (a->IsStatic() && b->IsStatic())
			{
				subIterator++;
				continue;
			}

			//Implement collision masks here
			
			float resolveDistance = 0;
			Vector2 resolveVector = Vector2();
			bool collided = false;

			//Check for collisions and assign to resolveDistance
			if (a->GetColliderType() == b->GetColliderType())
			{
				if (a->GetColliderType() == Collider::ColliderType::Polygon)
				{
					//Test polygons
					Vector2 vecAB, vecBA;
					float resolveAB, resolveBA;
					//Assign values to the smallest resolve values
					if (TestPolygons((ColliderPolygon*)a, (ColliderPolygon*)b, vecAB, resolveAB, false) && TestPolygons((ColliderPolygon*)b, (ColliderPolygon*)a, vecBA, resolveBA, true))
					{
						collided = true;
						if (abs(resolveAB) < abs(resolveBA))
						{
							resolveVector = vecAB;
							resolveDistance = resolveAB;
						}
						else
						{
							resolveVector = vecBA;
							resolveDistance = resolveBA;
						}
					}
				}
				else
				{
					if (TestCircles((ColliderCircle*)a, (ColliderCircle*)b, resolveVector, resolveDistance, false))
					{
						collided = true;
					}
				}
			}
			else
			{
				if (a->GetColliderType() == Collider::ColliderType::Circle)
				{
					//TestCircleToPolygon(a, b)
				}
				else
				{
					//TestCircleToPolygon(b, a)
				}
			}

			if (collided)
			{
				float restitution = (a->GetRestitution() + b->GetRestitution()) * 0.5f;

				//Generate collision
				Collision col = Collision();
				col.other = b;
				col.resolveVector = resolveVector;
				col.resolveDistance = resolveDistance;
				col.relativePosition = a->GetEntity()->GetTransform()->GetGlobalPosition() - b->GetEntity()->GetTransform()->GetGlobalPosition();
				col.finalRestitution = restitution;
				col.finalMomentum = Vector2();
				PhysObject* aPhys = a->GetEntity()->GetComponent<PhysObject>();
				PhysObject* bPhys = b->GetEntity()->GetComponent<PhysObject>();
				if (aPhys != nullptr && bPhys != nullptr)
				{
					col.finalMomentum += aPhys->GetVelocity() / aPhys->GetInverseMass();
					col.finalMomentum += bPhys->GetVelocity() / bPhys->GetInverseMass();
				}

				//Pass collision to collider a
				a->GetEntity()->OnCollision(col);

				//Swap values for b and flip necessary values for b
				col.other = a;
				col.resolveVector = -resolveVector;
				col.relativePosition = -col.relativePosition;

				//Pass collision to collider b
				b->GetEntity()->OnCollision(col);
			}
			//Go to next element
			subIterator++;
		}
		//Go to next element
		iterator++;
	}
}

Vector2 CollisionManager::ProjectVertsMinMax(Vector2 axis, Vector2* verts, int vertCount)
{
	//Get base minMax
	Vector2 minMax = Vector2();
	minMax.x = Vector2::Dot(axis, verts[0]);
	minMax.y = minMax.x;

	float dot;

	//Go through all vertices, project them and override min if value is smaller, or override max if value is higher
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
		axis = (aVerts[(i + 1) % aVertCount] - aVerts[i]).RightAngle().Normalised();

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
		{
			//Clear memory
			delete[] aVerts;
			delete[] bVerts;
			return false;
		}

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
	}

	//Finally turn the vector into big man, so distance doesn't have to be calculated every time
	resolveVector *= resolveDistance;

	//Clear memory
	delete[] aVerts;
	delete[] bVerts;

	return true;
}

bool CollisionManager::TestCircles(ColliderCircle* a, ColliderCircle* b, Vector2& resolveVector, float& resolveDistance, bool flipResults)
{
	resolveVector = Vector2();
	resolveDistance = 0.0f;

	//Get b - a
	Vector2 abOffset = a->GetEntity()->GetTransform()->GetGlobalPosition() - b->GetEntity()->GetTransform()->GetGlobalPosition();
	
	float distance = abOffset.Magnitude();

	float combinedRadius= a->GetTransformedRadius() + b->GetTransformedRadius();

	//If combined radius is larger than the distance, then circles must be overlapping
	if (distance < combinedRadius)
	{
		//Difference between the total radius and distance is the amount needed to get outta there
		resolveDistance = combinedRadius - distance;

		//Divide by zero is bad so just make a unit vector going up
		if (distance == 0)
		{
			resolveVector = Vector2::Up();
			distance = 1.0f;
		}

		if (flipResults)
			resolveDistance = -resolveDistance;

		resolveVector = abOffset * resolveDistance / -distance;

		return true;
	}

	return false;
}

bool CollisionManager::TestPolygonToCircle(ColliderPolygon* poly, ColliderCircle* circ, Vector2& resolveVector, float& resolveDistance, bool flipResults)
{
	return false;
}

bool CollisionManager::RayCast(Vector2 origin, Vector2 direction, float maxDistance, RayHit& rayHit)
{
	
	return false;
	
}

bool CollisionManager::RayCastToPolygon(ColliderPolygon* collider, Vector2 origin, Vector2 direction, float maxDistance, RayHit& rayHit)
{
	Vector2 p1 = origin;
	Vector2 p2 = origin + (direction * maxDistance);

	Vector2* points;

	int pointCount = collider->GetTransformedPoints(points);

	Vector2* normals = new Vector2[pointCount];

	//Generate normals
	for (int i = 0; i < pointCount; ++i)
	{
		normals[i] = (points[(i + 1) % pointCount] - points[i]).RightAngle().Normalised();
	}



	delete normals;
	delete points;

	return false;
}

void CollisionManager::DrawColliders(aie::Renderer2D* renderer)
{
	for (Collider* col : colliders)
	{
		col->Draw(renderer);
	}
}
