#include "BehaviourSequence.h"

BehaviourSequence::~BehaviourSequence()
{
}

BehaviourState BehaviourSequence::Execute(AIAgent* agent)
{
	BehaviourState behaviourState;
	for (auto child : children)
	{
		behaviourState = child->Execute(agent);

		//Returns failure or pending
		if (behaviourState != BehaviourState::BehaviourStateSuccess)
			return behaviourState;
	}

	return BehaviourState::BehaviourStateSuccess;
}
