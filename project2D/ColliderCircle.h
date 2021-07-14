#pragma once
#include "Collider.h"
class ColliderCircle :
	public Collider
{
public:
	using Collider::Collider;
	~ColliderCircle() {};

	float GetRadius();
	void SetRadius(float value);

	float GetTransformedRadius();

	ColliderType GetColliderType() override;

	void Draw(aie::Renderer2D* renderer) override;

protected:
	float radius = 1.0f;
};

