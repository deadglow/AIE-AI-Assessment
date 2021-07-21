#pragma once
#include "BehaviourComposite.h"

class BehaviourSequence :
    public BehaviourComposite
{
public:
    using BehaviourComposite::BehaviourComposite;
    ~BehaviourSequence();

    BehaviourState Execute(AIAgent* agent) override;
protected:
};

