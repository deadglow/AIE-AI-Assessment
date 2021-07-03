#pragma once
#include "Entity.h"

class Component
{
public:
	Component(Entity* entity);
	~Component();
	
	Entity* GetEntity();

	virtual void Initialise() = 0;
	virtual void Start() {};
	virtual void Update() {};
	virtual void OnCollision() {};
	
protected:
	Entity* entity;

};
