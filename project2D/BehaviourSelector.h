#pragma once
#include "BehaviourComposite.h"

//Returns success when any of its children returns true
class BehaviourSelector :
    public BehaviourComposite
{
public:
    using BehaviourComposite::BehaviourComposite;
    virtual ~BehaviourSelector();

    BehaviourState Execute(AIAgent* agent) override;
protected:
};

