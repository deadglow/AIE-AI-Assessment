#include "Sprite.h"
#include "Entity.h"
#include "Application.h"
#include <iostream>

Sprite* Sprite::CloneTo(Entity* ent)
{
	//Copy all this shiiiiii
	Sprite* newSprite = ent->AddComponent<Sprite>();
	newSprite->anim = this->anim;
	newSprite->frameIndex = this->frameIndex;
	newSprite->frameRate = this->frameRate;
	newSprite->frameTimer = this->frameTimer;
	newSprite->rotation = this->rotation;
	newSprite->depth = this->depth;
	newSprite->tint = this->tint;
	newSprite->flipX = this->flipX;
	newSprite->flipY = this->flipY;

	return newSprite;
}

void Sprite::Update()
{
	float deltaTime = aie::Application::GetInstance()->GetDeltaTime();
	frameTimer += deltaTime;

	frameIndex = (int)(frameTimer * frameRate) % anim->GetFrameCount();

	entity->GetGameData()->AddSpriteDrawCall(this);
}

void Sprite::Draw()
{
	aie::Renderer2D* renderer = entity->GetGameData()->GetRenderer();
	renderer->SetRenderColour(tint);
	renderer->DrawSpriteTransformed3x3(anim->GetFrame(frameIndex), &(entity->GetTransform()->GetGlobalTransform().m[0]), 0.0f, 0.0f, this->depth);
}

Animation* Sprite::GetAnimation()
{
	return this->anim;
}

void Sprite::SetAnimation(Animation* anim)
{
	this->anim = anim;
}

float Sprite::GetRotation()
{
	return this->rotation;
}

void Sprite::SetRotation(float rotation)
{
	this->rotation = rotation;
}

float Sprite::GetDepth()
{
	return this->depth;
}


void Sprite::SetDepth(float depth)
{
	this->depth = depth;
}

unsigned int Sprite::GetTint()
{
	return this->tint;
}

void Sprite::SetTint(unsigned int tint)
{
	this->tint = tint;
}

bool Sprite::GetFlipX()
{
	return this->flipX;
}

bool Sprite::GetFlipY()
{
	return this->flipY;
}

void Sprite::SetFlipX(bool flip)
{
	this->flipX = flip;
}

void Sprite::SetFlipY(bool flip)
{
	this->flipY = flip;
}