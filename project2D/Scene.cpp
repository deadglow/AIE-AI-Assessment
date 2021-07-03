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
		delete transform->GetChild(i)->GetEntity();
	}
	delete transform;
}

Transform* Scene::GetTransform()
{
	return transform;
}

std::string Scene::GetName()
{
	return name;
}

Entity* Scene::CreateEntity()
{
	Entity* newEnt = new Entity();
	newEnt->GetTransform()->SetParent(transform);
	return newEnt;
}

void Scene::Update()
{
	for (Transform* t : *transform->_GetChildrenList())
	{
		t->GetEntity()->Update();
	}
}

void Scene::SetName(std::string newName)
{
	name = newName;
}