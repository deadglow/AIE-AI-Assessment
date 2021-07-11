#pragma once

#include "Component.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Transform.h"

class Player : public Component
{
public:
	Player(Entity* entity);
	~Player();

	Player* CloneTo(Entity* ent) override;

	void Update() override;

private:
	float speed = 150.0f;
	Transform* targeter;

};

