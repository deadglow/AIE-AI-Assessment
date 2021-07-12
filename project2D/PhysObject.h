#pragma once
#include "Vector2.h"
#include "Component.h"

using namespace MathMan;

class Collider;

class PhysObject : public Component
{
public:
	using Component::Component;

	PhysObject* CloneTo(Entity* ent);

	void OnCollision(Collision collision) override;
	void Update() override;

	void AddForce(Vector2 force);
	
	float GetMass();
	void SetMass(float newMass);

	float GetDrag();
	void SetDrag(float newDrag);

	float GetAngularDrag();
	void SetAngularDrag(float angularDrag);

	Vector2 GetVelocity();
	void SetVelocity(Vector2 vel);

	float GetAngularVelocity();
	void SetAngularVelocity(float angularVel);
protected:
	float mass = 1.0f;
	float drag = 0.3f;
	float angularDrag = 0.3f;
	Vector2 velocity = { 0.0f, 0.0f };
	float angularVelocity = 0.0f;
};