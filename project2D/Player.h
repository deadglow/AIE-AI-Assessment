#pragma once

#include "Component.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Transform.h"

class Player : public Component
{
public:
	using Component::Component;

	~Player();

	Player* CloneTo(Entity* ent) override;

	void Update() override;

	void SetTargeter(Transform* t);

private:
	Transform* targeter = nullptr;
	float speed = 150.0f;

};

