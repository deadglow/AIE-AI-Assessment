#pragma once
#include <functional>
#include "Component.h"
#include "Renderer2D.h"
#include "Animation.h"

class Sprite : public Component
{
public:
	using Component::Component;

	~Sprite() {};

	Sprite* CloneTo(Entity* ent) override;
	
	void Update() override;
	void Draw();

	Animation* GetAnimation();
	void SetAnimation(Animation* tex);

	float GetRotation();
	void SetRotation(float rotation);

	float GetDepth();
	void SetDepth(float depth);

	unsigned int GetTint();
	void SetTint(unsigned int tint);

	bool GetFlipX();
	bool GetFlipY();

	void SetFlipX(bool flip);
	void SetFlipY(bool flip);
protected:
	Animation* anim;
	int frameIndex;
	float frameRate;
	float frameTimer;
	float rotation = 0.0f;
	float depth = 0.0f;
	unsigned int tint = UINT32_MAX;
	bool flipX = false;
	bool flipY = false;
};