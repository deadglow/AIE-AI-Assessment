#pragma once

#include "Component.h"
#include "Renderer2D.h"
#include "Texture.h"

class Player : public Component
{
public:
	using Component::Component;
	~Player();
	void Update() override;

private:
	float speed = 150.0f;
	float rotationSpeed = 90.0f;
	float minScale = 1.0f;
	float maxScaleOffset = 0.2f;
	float scaleFrequency = 2.0f;
};

