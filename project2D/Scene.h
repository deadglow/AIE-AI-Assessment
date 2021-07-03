#pragma once
#include <string>
#include "Transform.h"
#include "Entity.h"

class Scene
{
public:
	Scene();
	~Scene();
	Transform* GetTransform();
	std::string GetName();
	Entity* CreateEntity();
	void Update();
	void SetName(std::string newName);

protected:
	std::string name = "Scene";
	Transform* transform;
};