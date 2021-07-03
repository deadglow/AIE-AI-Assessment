#include "Entity.h"

Entity::Entity()
{
	transform = AddComponent<Transform>();
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

Transform* Entity::GetTransform()
{
	return transform;
}

int Entity::GetComponentCount()
{
	return components.size();
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