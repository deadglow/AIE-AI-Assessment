#include "Scene.h"

Scene::Scene()
{
	transform = new Transform(nullptr);
}

Scene::~Scene()
{
	int count = transform->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		delete transform->GetChild(i);
	}
}

Transform* Scene::GetRoot()
{
	return transform;
}

std::string Scene::GetName()
{
	return name;
}

void Scene::SetName(std::string newName)
{
	name = newName;
}