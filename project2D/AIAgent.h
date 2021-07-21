#pragma once
#include "Component.h"
class AIAgent :
    public Component
{
public:
	using Component::Component;

	~AIAgent();

	AIAgent* CloneTo(Entity* ent) override;
protected:

};

