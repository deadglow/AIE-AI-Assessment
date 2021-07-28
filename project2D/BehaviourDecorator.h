#pragma once
#include "Behaviour.h"

//Decorators modify the child's behaviourstate
class BehaviourDecorator :
    public Behaviour
{
public:
    BehaviourDecorator(std::string name) : Behaviour::Behaviour(name) {};
    virtual ~BehaviourDecorator();

    virtual BehaviourState Execute(AIAgent* agent) override = 0;

    void SetChild(Behaviour* child);
protected:
    Behaviour* child = nullptr;
};

