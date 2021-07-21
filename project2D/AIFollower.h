#pragma once
#include "Component.h"
class AIFollower :
    public Component
{
public:
	using Component::Component;

	~AIFollower();

	AIFollower* CloneTo(Entity* ent) override;
	void Update() override;

protected:
	float acceleration = 150.0f;
	Vector2 direction = Vector2();
};

