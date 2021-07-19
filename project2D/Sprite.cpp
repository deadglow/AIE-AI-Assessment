#include "Sprite.h"
#include "Entity.h"
#include "Application.h"
#include <iostream>
#include <cmath>

Sprite* Sprite::CloneTo(Entity* ent)
{
	//Copy all this shiiiiii
	Sprite* newSprite = ent->AddComponent<Sprite>();
	newSprite->anim = this->anim;
	newSprite->frameIndex = this->frameIndex;
	newSprite->frameRate = this->frameRate;
	newSprite->progress = this->progress;
	newSprite->rotation = this->rotation;
	newSprite->depth = this->depth;
	newSprite->tint = this->tint;
	newSprite->flipX = this->flipX;
	newSprite->flipY = this->flipY;

	return newSprite;
}

void Sprite::Update()
{
	progress += (aie::Application::GetInstance()->GetDeltaTime() * frameRate) / anim->GetFrameCount();
	progress -= std::floorf(progress);

	frameIndex = (int)(progress * anim->GetFrameCount());

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

int Sprite::GetFrameIndex()
{
	return frameIndex;
}

void Sprite::SetFrameIndex(int index)
{
	frameIndex = index;
}

float Sprite::GetFrameRate()
{
	return frameRate;
}

void Sprite::SetFrameRate(float fr)
{
	frameRate = fr;
}

float Sprite::GetAnimationProgress()
{
	return progress;
}

void Sprite::SetAnimationProgress(float value)
{
	progress = value;
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