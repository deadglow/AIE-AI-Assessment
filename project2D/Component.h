#pragma once
#include "Game2D.h"

class Entity;

class Component
{
public:
	Component(Entity* entity);
	~Component() {};
	
	Entity* GetEntity();

	virtual void Initialise() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void OnCollision() {};
	virtual void OnDestroy() {};
	
protected:
	Entity* entity;

};
