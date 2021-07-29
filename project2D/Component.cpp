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

bool Component::IsEnabled()
{
	return enabled;
}

void Component::SetEnabled(bool state)
{
	enabled = state;
}
