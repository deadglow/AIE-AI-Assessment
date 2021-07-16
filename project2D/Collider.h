#pragma once
#include "Component.h"

class PhysObject;

class Collider : public Component
{
public:
	enum class ColliderType
	{
		Default,
		Polygon,
		Circle
	};

	Collider(Entity* ent);
	virtual ~Collider();

	virtual Collider* CloneTo(Entity* ent) override;

	bool IsStatic();
	void SetStatic(bool value);
	float GetRestitution();
	void SetRestitution(float value);
	virtual ColliderType GetColliderType();

	virtual void Draw(aie::Renderer2D* renderer) {};

protected:
	bool isStatic = false;
	float restitution = 0.0f;
	PhysObject* attachedPhysObject = nullptr;


};
