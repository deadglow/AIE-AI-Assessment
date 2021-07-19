#pragma once
#include <functional>
#include "Component.h"
#include "Renderer2D.h"
#include "Animation.h"

#define ANIMATION_FRAMERATE 20

class Sprite : public Component
{
public:
	using Component::Component;

	~Sprite() {};

	Sprite* CloneTo(Entity* ent) override;
	
	void Update() override;
	void Draw();

	Animation* GetAnimation();
	void SetAnimation(Animation* anim);

	//Gets current frame index
	int GetFrameIndex();
	void SetFrameIndex(int index);

	float GetFrameRate();
	void SetFrameRate(float fr);

	//0.0 - 1.0 value representing how far along the animation has progressed
	float GetAnimationProgress();
	void SetAnimationProgress(float value);

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
	//Animation stuff
	Animation* anim = nullptr;
	int frameIndex = 0;
	float frameRate = ANIMATION_FRAMERATE;
	float progress = 0.0f;

	//Sprite stuff
	float rotation = 0.0f;
	float depth = 0.0f;
	unsigned int tint = UINT32_MAX;
	bool flipX = false;
	bool flipY = false;
};