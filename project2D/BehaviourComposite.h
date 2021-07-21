#pragma once
#include "Behaviour.h"
#include <vector>

class BehaviourComposite : Behaviour
{
public:
	BehaviourComposite();
	virtual ~BehaviourComposite();

	virtual BehaviourState Execute(AIAgent* agent) override = 0;
	
	void AddChild(BehaviourComposite* node);

protected:
	std::vector<Behaviour*> children;
};