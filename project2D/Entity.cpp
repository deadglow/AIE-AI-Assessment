#include "Entity.h"

Entity::Entity()
{
	transform = new Transform(this->transform);
}

Entity::~Entity()
{
	int count = transform->GetChildCount();
	for (int i = 0; i < count; ++i)
	{
		delete transform->GetChild(i);
	}

	for (auto& iterator : components)
	{
		delete iterator.second;
		iterator.second = nullptr;
	}
}

int Entity::GetComponentCount()
{
	return components.size();
}

void Entity::Start()
{
	for (auto& iter : components)
		iter.second->Start();
}

void Entity::Update()
{
	for (auto& iter : components)
		iter.second->Update();
}

void Entity::OnCollision()
{
	for (auto& iter : components)
		iter.second->OnCollision();
}

std::string Entity::GetName()
{
	return name;
}

void Entity::SetName(std::string newName)
{
	name = newName;
}