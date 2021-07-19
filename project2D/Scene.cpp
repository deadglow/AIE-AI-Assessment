#include "Scene.h"
#include "Game2D.h"

Scene::Scene(Game2D* gameData)
{
	this->gameData = gameData;
	//Scene doesn't have a parent
	transform = new Transform(nullptr);
}

Scene::~Scene()
{
	//Delete all children in scene (which then delete their own children recursively)
	while (transform->GetChildCount() > 0)
	{
		delete transform->GetChild(0)->GetEntity();
	}

	delete transform;
}

Transform* Scene::GetTransform()
{
	return transform;
}

Entity* Scene::CreateEntity(Transform* parent)
{
	//Creates a new entity with the parent being the scene (the root)
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
	//Call on destroy before deleting the entity
	ent->OnDestroy();
	delete ent;
}

void Scene::Update()
{
	//Call main update loop on all children
	for (Transform* t : *transform->GetChildrenList())
	{
		t->GetEntity()->Update();
	}
}

std::string Scene::GetName()
{
	return name;
}

void Scene::SetName(std::string newName)
{
	name = newName;
}