#include "Sprite.h"
#include "Entity.h"

Sprite* Sprite::CloneTo(Entity* ent)
{
	//Copy all this shiiiiii
	Sprite* newSprite = ent->AddComponent<Sprite>();
	newSprite->tex = this->tex;
	newSprite->rotation = this->rotation;
	newSprite->depth = this->depth;
	newSprite->tint = this->tint;
	newSprite->flipX = this->flipX;
	newSprite->flipY = this->flipY;

	return newSprite;
}

void Sprite::Update()
{
	entity->GetGameData()->AddSpriteDrawCall(this);
}

void Sprite::Draw()
{
	aie::Renderer2D* renderer = entity->GetGameData()->GetRenderer();
	renderer->SetRenderColour(tint);
	renderer->DrawSpriteTransformed3x3(tex, &(entity->GetTransform()->GetGlobalTransform().m[0]), 0.0f, 0.0f, this->depth);
}

aie::Texture* Sprite::GetTexture()
{
	return this->tex;
}

void Sprite::SetTexture(aie::Texture* tex)
{
	this->tex = tex;
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