#include "BehaviourSelector.h"

BehaviourSelector::~BehaviourSelector()
{
}

BehaviourState BehaviourSelector::Execute(AIAgent* agent)
{
	BehaviourState behaviourState;

	//look for successes
	for (int i = startChild; i < children.size(); ++i)
	{
		behaviourState = children[i]->Execute(agent);
		if (behaviourState == BehaviourState::BehaviourStatePending)
		{
			//Return pending and set start child to pending child so its called first
			startChild = i;
			return behaviourState;
		}
		//Returns success
		if (behaviourState == BehaviourState::BehaviourStateSuccess)
		{
			//Reset start child
			startChild = 0;
			return behaviourState;
		}
	}

	//Reset start child
	startChild = 0;

	//No success, return failure
	return BehaviourState::BehaviourStateFailure;
}
