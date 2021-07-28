#pragma once
#include "Behaviour.h"
#include <functional>

//Returns success or failure depending on the query's response
class BehaviourQuery :
    public Behaviour
{
public:
    BehaviourQuery(std::string name, std::function<bool(AIAgent*)> query);
    virtual ~BehaviourQuery();

    BehaviourState Execute(AIAgent* agent) override;

protected:
    std::function<bool(AIAgent*)> query;
};

