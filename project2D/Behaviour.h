#pragma once

enum class BehaviourState
{
	BehaviourStateSuccess,
	BehaviourStateFailure,
	BehaviourStatePending
};

class AIAgent;

class Behaviour
{
public:
	Behaviour() {};
	virtual ~Behaviour() {};
	virtual BehaviourState Execute(AIAgent* agent) = 0;
};
