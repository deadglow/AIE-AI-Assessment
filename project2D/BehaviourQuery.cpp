#include "BehaviourQuery.h"

BehaviourQuery::BehaviourQuery(std::string name, std::function<bool(AIAgent*)> query) : Behaviour::Behaviour(name)
{
    this->query = query;
}

BehaviourQuery::~BehaviourQuery()
{
}

BehaviourState BehaviourQuery::Execute(AIAgent* agent)
{
    if (query(agent))
        return BehaviourState::BehaviourStateSuccess;
    else
        return BehaviourState::BehaviourStateFailure;
}
