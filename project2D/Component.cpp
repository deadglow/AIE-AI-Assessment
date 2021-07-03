#include "Component.h"
#include "Entity.h"

Component::Component(Entity* entity)
{
	this->entity = entity;
	Initialise();
}

Entity* Component::GetEntity()
{
	return entity;
}
