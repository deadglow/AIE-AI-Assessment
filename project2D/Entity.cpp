#include "Entity.h"
#include "Component.h"
#include "Transform.h"
#include "Scene.h"

Entity::Entity(Game2D* gameData)
{
	transform = AddComponent<Transform>();
	this->gameData = gameData;
}

Entity::~Entity()
{
	int count = transform->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		delete transform->GetChild(i)->GetEntity();
	}

	for (auto& iterator : components)
	{
		delete iterator.second;
		iterator.second = nullptr;
	}
}

Transform* Entity::GetTransform()
{
	return transform;
}

Game2D* Entity::GetGameData()
{
	return gameData;
}

size_t Entity::GetComponentCount()
{
	return components.size();
}

Entity* Entity::Clone()
{
	//Create new entity and clone each of this object's components to it
	Entity* newEnt = new Entity(this->gameData);
	
	for (const auto& component : components)
	{
		component.second->CloneTo(newEnt);
	}

	//Get number of children
	int childCount = transform->GetChildCount();
	
	//Loop through, clone all children and set their parent to the new clone.
	//This is recursive, so all children's children will be cloned and so on.
	for (int i = 0; i < childCount; ++i)
	{
		Entity* clone = transform->GetChild(i)->GetEntity()->Clone();
		clone->transform->SetParent(newEnt->transform);
	}

	return newEnt;
}

void Entity::Start()
{
	for (auto& iter : components)
		iter.second->Start();

	for (Transform* t : *transform->_GetChildrenList())
	{
		t->GetEntity()->Start();
	}
}

void Entity::Update()
{
	for (auto& iter : components)
		iter.second->Update();

	for (Transform* t : *transform->_GetChildrenList())
	{
		t->GetEntity()->Update();
	}
}

void Entity::OnCollision(Collision collision)
{
	for (auto& iter : components)
		iter.second->OnCollision(collision);

	for (Transform* t : *transform->_GetChildrenList())
	{
		t->GetEntity()->OnCollision(collision);
	}
}

std::string Entity::GetName()
{
	return name;
}

void Entity::SetName(std::string newName)
{
	name = newName;
}

int Entity::GetLayer()
{
	return layer;
}

void Entity::SetLayer(int newLayer)
{
	layer = newLayer;
}

Entity* Entity::CreateEntity(Transform* parent)
{
	return gameData->GetMainScene()->CreateEntity(parent);
}

Entity* Entity::CreateEntity()
{
	return gameData->GetMainScene()->CreateEntity();
}