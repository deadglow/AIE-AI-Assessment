#pragma once
#include "Collider.h"
class ColliderCircle :
	public Collider
{
public:
	using Collider::Collider;

	float GetRadius();
	void SetRadius(float value);

	float GetTransformedRadius();

	ColliderType GetColliderType() override;

protected:
	float radius = 1.0f;
};

