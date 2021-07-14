#include "ColliderCircle.h"
#include "Entity.h"

float ColliderCircle::GetRadius()
{
	return radius;
}

void ColliderCircle::SetRadius(float value)
{
	radius = value;
}

float ColliderCircle::GetTransformedRadius()
{
	Vector2 scale = entity->GetTransform()->GetLocalScale();
	return radius * std::fmaxf(scale.x, scale.y);
}

Collider::ColliderType ColliderCircle::GetColliderType()
{
	return Collider::ColliderType::Circle;
}

void ColliderCircle::Draw(aie::Renderer2D* renderer)
{
	Vector2 pos = entity->GetTransform()->GetGlobalPosition();
	renderer->DrawCircle(pos.x, pos.y, GetTransformedRadius(), -1.0f);
}
