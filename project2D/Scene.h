#pragma once
#include <string>
#include "Transform.h"
#include "Entity.h"
#include "Game2D.h"

class Scene
{
public:
	Scene(Game2D* gameData);
	~Scene();
	Transform* GetTransform();
	Entity* CreateEntity(Transform* parent);
	Entity* CreateEntity();
	void DestroyEntity(Entity* ent);
	void Update();
	std::string GetName();
	void SetName(std::string newName);

protected:
	//Pointer to refer back to game2d
	Game2D* gameData;
	std::string name;
	Transform* transform;
};