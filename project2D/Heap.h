#pragma once
#include <vector>
#include "PathfinderNode.h"

class Heap
{
public:
	void Push(PathfinderNode* value);
	PathfinderNode* Pop();
	PathfinderNode* Peek();
	int Find(PathfinderNode* node);
	void Rebalance(PathfinderNode* node);
	int Size();
	void Clear();

	PathfinderNode*& operator [](int index)
	{
		return data[index];
	}

private:
	int GetParentIndex(int index);
	int GetChild1Index(int index);
	int GetChild2Index(int index);
	std::vector<PathfinderNode*> data;
};