#include "Entity.h"

Entity::Entity()
{
	transform = new Transform(this);
}

Entity::~Entity()
{
	for (auto& iterator : components)
	{
		delete iterator.second;
		iterator.second = nullptr;
	}
}