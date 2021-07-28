#include "PhysObject.h"
#include "Collider.h"
#include "Entity.h"
#include "Application.h"

PhysObject* PhysObject::CloneTo(Entity* ent)
{
	PhysObject* phys = ent->AddComponent<PhysObject>();
	
	phys->inverseMass = this->inverseMass;
	phys->drag = this->drag;
	phys->angularDrag = this->angularDrag;
	phys->velocity = this->velocity;
	phys->angularVelocity = this->angularVelocity;

	return phys;
}

void PhysObject::OnCollision(Collision collision)
{
	Vector2 resolve = collision.resolveVector;
	//Get outta there
	if (!collision.other->IsStatic())
		resolve /= 2;

	entity->GetTransform()->Translate(resolve, false);

	//Figure out inelastic collision stuff
	//velocity = collision.finalMomentum * inverseMass;

	//Fix elastic collisions too
	Vector2 resolveAxis = -collision.resolveVector / abs(collision.resolveDistance);
	float dot = Vector2::Dot(velocity, resolveAxis);
	if (dot > 0)
		velocity -= resolveAxis * (dot + dot * collision.finalRestitution);
}

void PhysObject::Update()
{
	float deltaTime = aie::Application::GetInstance()->GetDeltaTime();
	//Apply drag
	velocity -= velocity * std::min(drag * deltaTime, 1.0f);
	angularVelocity -= angularVelocity * std::min(angularDrag * deltaTime, 1.0f);

	//Limit velocity
	if (velocity.SqrMagnitude() > PHYS_MAX_SPEED * PHYS_MAX_SPEED)
		velocity = velocity.Normalised() * PHYS_MAX_SPEED;

	//Update position
	entity->GetTransform()->Rotate(angularVelocity * (float)DEG2RAD * deltaTime);
	entity->GetTransform()->Translate(velocity * deltaTime);
}

void PhysObject::AddForce(Vector2 force)
{
	velocity += force * inverseMass;
}

float PhysObject::GetInverseMass()
{
	return inverseMass;
}

void PhysObject::SetInverseMass(float newMass)
{
	inverseMass = newMass;
}

float PhysObject::GetDrag()
{
	return drag;
}

void PhysObject::SetDrag(float newDrag)
{
	drag = newDrag;
}

float PhysObject::GetAngularDrag()
{
	return angularDrag;
}

void PhysObject::SetAngularDrag(float newDrag)
{
	angularDrag = newDrag;
}

Vector2 PhysObject::GetVelocity()
{
	return velocity;
}

void PhysObject::SetVelocity(Vector2 vel)
{
	velocity = vel;
}

float PhysObject::GetAngularVelocity()
{
	return angularVelocity;
}

void PhysObject::SetAngularVelocity(float angularVel)
{
	angularVelocity = angularVel;
}