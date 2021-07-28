#pragma once
#include "Behaviour.h"
#include <vector>
#include <string>

//Base class for selectors and sequences
class BehaviourComposite : public Behaviour
{
public:
	using Behaviour::Behaviour;
	virtual ~BehaviourComposite();

	virtual BehaviourState Execute(AIAgent* agent) override = 0;
	
	Behaviour* AddChild(Behaviour* node);

	Behaviour* GetChild(int index);

protected:
	std::vector<Behaviour*> children;
	//Used for pending behaviour. Will change to the index of the pending behaviour
	int startChild = 0;
};