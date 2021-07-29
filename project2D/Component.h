#pragma once
#include "Game2D.h"
#include "Collision.h"

class Entity;

//Base class for all components
class Component
{
public:
	Component(Entity* entity);
	virtual ~Component() {};
	
	Entity* GetEntity();

	virtual void Start() {};
	virtual void Update() {};
	virtual void OnCollision(Collision collision) {};
	virtual void OnDestroy() {};
	//Used when entity is cloned
	virtual Component* CloneTo(Entity* ent) = 0;
	
	bool IsEnabled();
	void SetEnabled(bool state);

protected:
	//Attached entity
	Entity* entity;
	bool enabled = true;
};
