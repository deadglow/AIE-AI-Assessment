#include "BehaviourDecorator.h"

BehaviourDecorator::~BehaviourDecorator()
{
	delete child;
}

void BehaviourDecorator::SetChild(Behaviour* child)
{
	this->child = child;
}