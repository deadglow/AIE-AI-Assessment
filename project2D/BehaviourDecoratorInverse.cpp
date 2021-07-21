#include "BehaviourDecoratorInverse.h"

BehaviourState BehaviourDecoratorInverse::Execute(AIAgent* agent)
{
    BehaviourState behaviourState = child->Execute(agent);

    switch (behaviourState)
    {
    case BehaviourState::BehaviourStateSuccess:
        return BehaviourState::BehaviourStateFailure;
        break;

    case BehaviourState::BehaviourStateFailure:
        return BehaviourState::BehaviourStateSuccess;
        break;

    default:
        return BehaviourState::BehaviourStatePending;
    }
}
