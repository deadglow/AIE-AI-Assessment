#include "Heap.h"

int Heap::GetParentIndex(int index)
{
	return (index - 1) / 2;
}

int Heap::GetChild1Index(int index)
{
	return 2 * index + 1;
}

int Heap::GetChild2Index(int index)
{
	return 2 * index + 2;
}

int Heap::Size()
{
	return this->data.size();
}

void Heap::Clear()
{
	this->data.clear();
}

PathfinderNode* Heap::Peek()
{
	return data[0];
}

int Heap::Find(PathfinderNode* node)
{
	//find index
	auto iter = std::find(data.begin(), data.end(), node);

	if (iter == data.end())
		return -1;
	int index = iter - data.begin();

	if (index >= data.size())
		return -1;

	return index;
}

void Heap::Push(PathfinderNode* value)
{
	//Add value to the array
	this->data.push_back(value);
	//Set index to the last element in the array (the value we just added)
	int curIndex = this->data.size() - 1;
	int parentIndex = GetParentIndex(curIndex);
	
	//	--Swap curIndex value with its parent if the value is smaller than the parent
	//	--Changes curIndex to be the parent's index and then finds new parent
	//	--Breaks when parentIndex goes under 0 (root) or until the parent is no longer
	//		larger than its children
	while (parentIndex >= 0 && data[parentIndex]->gScore > data[curIndex]->gScore)
	{
		data[curIndex] = data[parentIndex];
		data[parentIndex] = value;
		curIndex = parentIndex;
		parentIndex = GetParentIndex(curIndex);
	}
}

void Heap::Rebalance(PathfinderNode* node)
{
	//Set index to the last element in the array (the value we just added)
	int curIndex = Find(node);

	if (curIndex < 0)
		return;

	int parentIndex = GetParentIndex(curIndex);

	//	--Swap curIndex value with its parent if the value is smaller than the parent
	//	--Changes curIndex to be the parent's index and then finds new parent
	//	--Breaks when parentIndex goes under 0 (root) or until the parent is no longer
	//		larger than its children
	while (parentIndex >= 0 && data[parentIndex]->gScore > data[curIndex]->gScore)
	{
		data[curIndex] = data[parentIndex];
		data[parentIndex] = node;
		curIndex = parentIndex;
		parentIndex = GetParentIndex(curIndex);
	}
}

PathfinderNode* Heap::Pop()
{
	if (data.size() < 1)
		throw "Heap is empty!";

	//Caches removedValue to be returned later
	PathfinderNode* removedValue = this->data[0];
	//Replace root with the last element 3in the array (larger element) and delete last element
	this->data[0] = data[data.size() - 1];
	data.pop_back();

	int curIndex = 0;
	int smallestChildIndex = GetChild1Index(0);
	int otherChildIndex = GetChild2Index(0);

	//Shifts root value down until it is no longer larger than its children
	//While loop stops when curIndex goes out of bounds or until shifting is complete
	while (curIndex < data.size())
	{
		//Child1 is the default target child
		smallestChildIndex = GetChild1Index(curIndex);
		otherChildIndex = GetChild2Index(curIndex);

		//If child1 is out of bounds, then child2 must also be out of bounds so break the loop
		if (smallestChildIndex < data.size())
		{
			//Replace child1index with child2index if child2 exists and is smaller than child1
			if (otherChildIndex < data.size() && data[smallestChildIndex]->gScore > data[otherChildIndex]->gScore)
				smallestChildIndex = otherChildIndex;
		}
		else
			break;

		//Swap parent and child data if parent is larger
		//Otherwise loop breaks because reordering is done
		if (data[curIndex]->gScore > data[smallestChildIndex]->gScore)
		{
			PathfinderNode* temp = data[curIndex];
			data[curIndex] = data[smallestChildIndex];
			data[smallestChildIndex] = temp;
			curIndex = smallestChildIndex;
		}
		else
			break;
	}

	//Here you are sir
	return removedValue;
}
