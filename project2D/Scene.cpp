#include "Scene.h"
#include "Game2D.h"

Scene::Scene(Game2D* gameData)
{
	this->gameData = gameData;
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

Entity* Scene::CreateEntity(Transform* parent)
{
	Entity* newEnt = new Entity(gameData);
	newEnt->GetTransform()->SetParent(parent);
	return newEnt;
}

Entity* Scene::CreateEntity()
{
	return CreateEntity(this->transform);
}

void Scene::DestroyEntity(Entity* ent)
{
	ent->GetTransform()->SetParent(nullptr);
	delete ent;
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