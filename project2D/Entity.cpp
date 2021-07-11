#include "Entity.h"
#include "Component.h"
#include "Transform.h"

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

int Entity::GetComponentCount()
{
	return components.size();
}

Entity* Entity::Clone()
{
	Entity* newEnt = new Entity(this->gameData);
	
	for (const auto& component : components)
	{
		newEnt->components.insert({typeid(decltype(component.second)), new decltype(component.second)(component.second) });
	}
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

void Entity::OnCollision()
{
	for (auto& iter : components)
		iter.second->OnCollision();

	for (Transform* t : *transform->_GetChildrenList())
	{
		t->GetEntity()->OnCollision();
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