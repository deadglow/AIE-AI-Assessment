#include "Throwable.h"
#include "Entity.h"
#include "AIManager.h"
#include "Scene.h"
#include <iostream>

Throwable::~Throwable()
{
}

Throwable* Throwable::CloneTo(Entity* ent)
{
	Throwable* throwable = ent->AddComponent<Throwable>();
	throwable->soundRadius = this->soundRadius;
	throwable->soundLoudness = this->soundLoudness;
	return throwable;
}

void Throwable::Update()
{
}

void Throwable::OnCollision(Collision collision)
{
	if (collision.finalMomentum > IMPACT_FORCE)
	{
		//entity->GetGameData()->StartSlowMotion(SLOWMO_SCALE, SLOWMO_DURATION);
		//Make sound
		Sound sound = Sound(entity->GetTransform()->GetGlobalPosition() - (collision.resolveVector * 2), soundLoudness, soundRadius);
		entity->GetGameData()->GetAIManager()->CheckSound(sound);
	}
}

void Throwable::Throw(Vector2 velocity, float spin)
{
	Drop();
	//Add impulse and spin
	PhysObject* phys = entity->GetComponent<PhysObject>();
	phys->SetVelocity(velocity);
	phys->SetAngularVelocity(spin);
}

void Throwable::Drop()
{
	Transform* transform = entity->GetTransform();
	//Set parent to root and set position to be in front of old parent
	transform->SetLocalTransform(transform->GetParent()->GetGlobalTransform() * transform->GetLocalTransform());
	transform->SetParent(entity->GetGameData()->GetMainScene()->GetTransform());

	//Enable collider
	entity->GetComponent<ColliderPolygon>()->SetEnabled(true);

	//Enable physics and set velocity
	PhysObject* phys = entity->GetComponent<PhysObject>();
	phys->SetEnabled(true);
	phys->SetVelocity(Vector2());
}

void Throwable::PickUp(Transform* parent, Vector2 offset)
{
	//Disable collider and physobject
	entity->GetComponent<ColliderPolygon>()->SetEnabled(false);
	entity->GetComponent<PhysObject>()->SetEnabled(false);

	//Set parent to holder and apply offset
	Transform* transform = entity->GetTransform();
	transform->SetParent(parent);
	transform->SetLocalPosition(offset);
}
