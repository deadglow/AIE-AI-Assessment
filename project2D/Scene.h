#pragma once
#include <string>
#include "Transform.h"

class Scene
{
public:
	Scene();
	~Scene();
	Transform* GetRoot();
	std::string GetName();
	void SetName(std::string newName);

protected:
	std::string name = "Scene";
	Transform* transform;
};