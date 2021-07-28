#include "BehaviourAction.h"

BehaviourAction::BehaviourAction(std::string name, std::function<bool(AIAgent*)> action) : Behaviour::Behaviour(name)
{
	this->action = action;
}

BehaviourAction::~BehaviourAction()
{
}

BehaviourState BehaviourAction::Execute(AIAgent* agent)
{
	//False from the action means the action is still occuring
	if (action(agent))
		return BehaviourState::BehaviourStateSuccess;
	else
		return BehaviourState::BehaviourStatePending;
}
