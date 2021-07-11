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
	std::string GetName();
	Entity* CreateEntity(Transform* parent);
	Entity* CreateEntity();
	void Update();
	void SetName(std::string newName);

protected:
	Game2D* gameData;
	std::string name;
	Transform* transform;
};