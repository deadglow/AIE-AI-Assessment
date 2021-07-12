#pragma once
#include "Component.h"

class PhysObject;

enum ColliderType : int
{
	Default,
	Polygon,
	Circle
};

class Collider : public Component
{
public:
	Collider(Entity* ent);

	Collider* CloneTo(Entity* ent);

	bool IsStatic();
	void SetStatic(bool value);
	float GetRestitution();
	void SetRestitution(float value);
	virtual ColliderType GetColliderType() = 0;

protected:
	bool isStatic = false;
	float restitution = 0.0f;
	PhysObject* attachedPhysObject = nullptr;


};
