#include "BehaviourSequence.h"

BehaviourSequence::~BehaviourSequence()
{
}

BehaviourState BehaviourSequence::Execute(AIAgent* agent)
{
	BehaviourState behaviourState;

	//Look for failures or pending
	for (int i = startChild; i < children.size(); ++i)
	{
		behaviourState = children[i]->Execute(agent);

		//Returns failure or pending
		if (behaviourState == BehaviourState::BehaviourStateFailure)
		{
			//Reset pending child
			startChild = 0;
			return behaviourState;
		}
		else if (behaviourState == BehaviourState::BehaviourStatePending)
		{
			//Change startChild to current pending node so that its run first (ensure that it's not pending anymore)
			startChild = i;
			return behaviourState;
		}
	}

	//Reset pending child
	startChild = 0;

	return BehaviourState::BehaviourStateSuccess;
}