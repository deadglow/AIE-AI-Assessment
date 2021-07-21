#include "BehaviourComposite.h"

BehaviourComposite::BehaviourComposite()
{
}

BehaviourComposite::~BehaviourComposite()
{
}

void BehaviourComposite::AddChild(BehaviourComposite* node)
{
	children.push_back(node);
}