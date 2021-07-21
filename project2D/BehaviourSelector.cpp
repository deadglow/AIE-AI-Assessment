#include "BehaviourSelector.h"

BehaviourSelector::~BehaviourSelector()
{
}

BehaviourState BehaviourSelector::Execute(AIAgent* agent)
{
	BehaviourState behaviourState;

	//Check for pending tasks first
	for (auto child : children)
	{
		behaviourState = child->Execute(agent);
		if (behaviourState == BehaviourState::BehaviourStatePending)
			return behaviourState;
	}

	//No pending was found, look for successes
	for (auto child : children)
	{
		behaviourState = child->Execute(agent);

		//Returns failure or pending
		if (behaviourState == BehaviourState::BehaviourStateSuccess)
			return behaviourState;
	}

	//No success, return failure
	return BehaviourState::BehaviourStateFailure;
}
