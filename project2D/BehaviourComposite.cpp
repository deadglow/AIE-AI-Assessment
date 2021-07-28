#include "BehaviourComposite.h"

BehaviourComposite::~BehaviourComposite()
{
	for (auto child : children)
	{
		delete child;
	}
	children.clear();
}

Behaviour* BehaviourComposite::AddChild(Behaviour* node)
{
	children.push_back(node);
	return node;
}

Behaviour* BehaviourComposite::GetChild(int index)
{
	return children[index];
}
