#pragma once

#include "Component.h"
#include "Renderer2D.h"
#include "Texture.h"
#include "Transform.h"

class Throwable;

#define THROW_VELOCITY 800.0f
#define THROW_SPIN 800.0f
#define PICKUP_DISTANCE 80.0f

class Player : public Component
{
public:
	using Component::Component;

	~Player();

	Player* CloneTo(Entity* ent) override;

	void Update() override;
	void OnCollision(Collision collision) override;

	//Assigns a targeter transform to the player
	void SetTargeter(Transform* t);

private:
	//Part of player that looks at mouse
	Transform* targeter = nullptr;
	//How fast the player can move (m/s)
	float speed = 300.0f;

	Throwable* heldThrowable = nullptr;
};

