#pragma once
#include "BehaviourComposite.h"

//Will return success only if all of its children also return success
class BehaviourSequence :
	public BehaviourComposite
{
public:
	using BehaviourComposite::BehaviourComposite;
	virtual ~BehaviourSequence();

	BehaviourState Execute(AIAgent* agent) override;
protected:
};