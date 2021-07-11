#pragma once
#include <functional>
#include "Component.h"
#include "Renderer2D.h"

class Sprite : public Component
{
public:
	using Component::Component;
	void Update() override;
	void Draw();

	aie::Texture* GetTexture();
	void SetTexture(aie::Texture* tex);

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
	aie::Texture* tex;
	float rotation = 0.0f;
	float depth = 0.0f;
	unsigned int tint = UINT32_MAX;
	bool flipX = false;
	bool flipY = false;
};