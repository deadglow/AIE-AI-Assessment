#pragma once
#include "BehaviourComposite.h"

class BehaviourSelector :
    public BehaviourComposite
{
public:
    using BehaviourComposite::BehaviourComposite;
    ~BehaviourSelector();

    BehaviourState Execute(AIAgent* agent) override;
protected:
};

