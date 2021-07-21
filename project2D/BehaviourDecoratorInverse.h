#pragma once
#include "BehaviourDecorator.h"
class BehaviourDecoratorInverse :
    public BehaviourDecorator
{
public:
    ~BehaviourDecoratorInverse() {};

    BehaviourState Execute(AIAgent* agent) override;
};