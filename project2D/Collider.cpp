#include "Collider.h"
#include "Entity.h"

Collider::Collider(Entity* ent) : Component::Component(ent)
{
	attachedPhysObject = ent->GetComponent<PhysObject>();
}

Collider* Collider::CloneTo(Entity* ent)
{
	Collider* col = ent->AddComponent<Collider>();
	col->isStatic = this->isStatic;
	col->restitution = this->restitution;
	//No need to assign attachedPhysObject since constructor does that
	
	return col;
}

bool Collider::IsStatic()
{
	return isStatic;
}

void Collider::SetStatic(bool value)
{
	isStatic = value;
}

float Collider::GetRestitution()
{
	return restitution;
}

void Collider::SetRestitution(float value)
{
	restitution = value;
}

Collider::ColliderType Collider::GetColliderType()
{
	return Collider::ColliderType::Default;
}