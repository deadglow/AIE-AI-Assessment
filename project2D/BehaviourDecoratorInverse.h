#pragma once
#include "BehaviourDecorator.h"

//Negates the behaviourstate of a behaviour
//Implemented just for tests
class BehaviourDecoratorInverse :
    public BehaviourDecorator
{
public:
    using BehaviourDecorator::BehaviourDecorator;
    virtual ~BehaviourDecoratorInverse() {};

    BehaviourState Execute(AIAgent* agent) override;
};