#include "Component.h"
#include "Entity.h"

Component::Component(Entity* entity)
{
	this->entity = entity;
}

Entity* Component::GetEntity()
{
	return entity;
}
