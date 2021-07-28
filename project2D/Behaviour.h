#pragma once
#include <string>
#include <iostream>

enum class BehaviourState
{
	BehaviourStateSuccess,
	BehaviourStateFailure,
	BehaviourStatePending
};

class AIAgent;

//Base class for all behaviour tree types
class Behaviour
{
public:
	Behaviour(std::string name);
	virtual ~Behaviour() {};
	virtual BehaviourState Execute(AIAgent* agent) = 0;
	std::string GetName();
	void PrintName();

protected:
	std::string name = "Behaviour";
};
