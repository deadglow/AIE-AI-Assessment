#pragma once
#include "Vector2.h"
#include "Component.h"

#define MAX_SPEED 1200.0f

using namespace MathMan;

class Collider;

class PhysObject : public Component
{
public:
	using Component::Component;

	~PhysObject() {};

	PhysObject* CloneTo(Entity* ent);

	void OnCollision(Collision collision) override;
	void Update() override;

	void AddForce(Vector2 force);
	
	float GetInverseMass();
	void SetInverseMass(float value);

	float GetDrag();
	void SetDrag(float newDrag);

	float GetAngularDrag();
	void SetAngularDrag(float angularDrag);

	Vector2 GetVelocity();
	void SetVelocity(Vector2 vel);

	float GetAngularVelocity();
	void SetAngularVelocity(float angularVel);
protected:
	float inverseMass = 1.0f;
	float drag = 0.3f;
	float angularDrag = 0.3f;
	Vector2 velocity = { 0.0f, 0.0f };
	float angularVelocity = 0.0f;
};