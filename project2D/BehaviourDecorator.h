#pragma once
#include "Behaviour.h"
class BehaviourDecorator :
    public Behaviour
{
public:
    using Behaviour::Behaviour;
    virtual ~BehaviourDecorator() {};

    virtual BehaviourState Execute(AIAgent* agent) override = 0;

    void SetChild(Behaviour* child);
protected:
    Behaviour* child;
};

