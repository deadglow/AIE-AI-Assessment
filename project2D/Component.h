#pragma once

class Entity;

class Component
{
public:
	Component(Entity* entity);
	virtual ~Component() {};
	
	Entity* GetEntity();

	virtual void Initialise() {};
	virtual void Start() {};
	virtual void Update() {};
	virtual void OnCollision() {};
	
protected:
	Entity* entity;

};
