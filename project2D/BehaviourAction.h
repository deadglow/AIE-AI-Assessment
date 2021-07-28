#pragma once
#include "Behaviour.h"
#include <functional>

//Actions execute a behaviour and either return success or pending
class BehaviourAction :
    public Behaviour
{
public:
    BehaviourAction(std::string name, std::function<bool(AIAgent*)> action);
    virtual ~BehaviourAction();

    BehaviourState Execute(AIAgent* agent) override;

protected:
    std::function<bool(AIAgent*)> action;
};

