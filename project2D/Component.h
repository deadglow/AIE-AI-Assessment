#pragma once
#include "Game2D.h"
#include "Collision.h"

class Entity;

class Component
{
public:
	Component(Entity* entity);
	~Component() {};
	
	Entity* GetEntity();

	virtual void Start() {};
	virtual void Update() {};
	virtual void OnCollision(Collision collision) {};
	virtual void OnDestroy() {};
	virtual Component* CloneTo(Entity* ent) = 0;
	
protected:
	Entity* entity;

};
