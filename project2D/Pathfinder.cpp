#include "Pathfinder.h"

Pathfinder::Pathfinder(int width, int height, float gridSize)
{
	gridWidth = width;
	gridHeight = height;
	this->gridSize = gridSize;

	grid = new PathfinderNode* [width * height]();

	//init nodes
	PathfinderNode* node = nullptr;
	for (int y = 0; y < gridHeight; ++y)
	{
		for (int x = 0; x < gridWidth; ++x)
		{
			node = new PathfinderNode();
			node->x = x;
			node->y = y;
			grid[GetIndex(x, y)] = node;
		}
	}

	//Init neighbours
	for (int y = 0; y < gridHeight; ++y)
	{
		for (int x = 0; x < gridWidth; ++x)
		{
			node = GetNode(x, y);
			for (int i = 0; i < NEIGHBOUR_COUNT; ++i)
			{
				node->neighbours[i] = GetNeighbourFromIndex(x, y, i);
			}
		}
	}

	//Initialise closedList char array
	closedList = new char[width * height]();

	memset(closedList, 0, sizeof(char) * width * height);

	openList.Clear();
}

Pathfinder::~Pathfinder()
{
	for (int i = 0; i < gridWidth * gridHeight; ++i)
		delete grid[i];

	delete[] grid;

	openList.Clear();

	delete[] closedList;
}

PathfinderNode* Pathfinder::GetNode(int x, int y)
{
	return grid[GetIndex(x, y)];
}

PathfinderNode* Pathfinder::GetNode(int index)
{
	return grid[index];
}

PathfinderNode* Pathfinder::GetNodeFromPos(Vector2 pos)
{
	pos.x = (pos.x / gridSize) + 0.5f;
	pos.y = (pos.y / gridSize) + 0.5f;

	//Limit pos to the bounds of the grid
	pos.x = (float)fmax(0, fmin(pos.x, gridWidth - 1));
	pos.y = (float)fmax(0, fmin(pos.y, gridHeight - 1));

	return GetNode((int)pos.x, (int)pos.y);
}

Vector2 Pathfinder::GetPosFromNode(PathfinderNode* node)
{
	Vector2 vec;
	vec.x = (node->x + 0.5f) * gridSize;
	vec.y = (node->y + 0.5f) * gridSize;

	return vec;
}

PathfinderNode* Pathfinder::GetNeighbourFromIndex(int x, int y, int neighbour)
{
	int posX = x;
	int posY = y;

	switch (neighbour)
	{
	case 0: --posX;	++posY;	break;
	case 1:			++posY;	break;
	case 2: ++posX;	++posY;	break;
	case 3: --posX;			break;
	case 4: ++posX;			break;
	case 5: --posX;	--posY;	break;
	case 6:			--posY;	break;
	case 7: ++posX;	--posY;	break;
	}

	if (posY < 0 || posX >= gridWidth || posY < 0 || posY >= gridHeight)
	{
		return nullptr;
	}
	else
	{
		return GetNode(posX, posY);
	}
}

bool Pathfinder::CreateFlowField(SoundFieldNode*& outVecField, Vector2 targetPos)
{
	if (Dijkstras(GetNodeFromPos(targetPos)))
	{
		//Generate a new Vector field
		outVecField = new SoundFieldNode[gridHeight * gridWidth];
		
		//Caching
		PathfinderNode* node = nullptr;
		Vector2 vec;
		SoundFieldNode sfNode;

		for (int y = 0; y < gridHeight; ++y)
		{
			for (int x = 0; x < gridWidth; ++x)
			{
				node = GetNode(x, y);
				//If node has a previous node, make a vector that points to the previous node, otherwise make the vector zero
				if (node->previous != nullptr)
				{
					sfNode = SoundFieldNode();
					sfNode.direction = Vector2((float)node->previous->x - node->x, (float)node->previous->y - node->y);
					sfNode.direction = sfNode.direction.Normalised();
					sfNode.gScore = node->gScore;
				}
				else
				{
					sfNode = SoundFieldNode();
					sfNode.direction = Vector2();
				}
				outVecField[GetIndex(x, y)] = sfNode;
			}
		}

		return true;
	}
	else
	{
		return false;
	}
}

int Pathfinder::GetWidth()
{
	return gridWidth;
}

int Pathfinder::GetHeight()
{
	return gridHeight;
}

bool Pathfinder::Dijkstras(PathfinderNode* startNode)
{
	if (startNode->blocked)
		return false;

	openList.Clear();

	//clear blockedlist
	memset(closedList, 0, sizeof(char) * gridWidth * gridHeight);

	startNode->previous = nullptr;
	startNode->gScore = 0;

	openList.Push(startNode);

	//Caching
	PathfinderNode* current;
	PathfinderNode* neighbour;

	while (openList.Size() > 0)
	{
		current = openList.Pop();

		closedList[GetIndex(current->x, current->y)] = true;

		for (int n = 0; n < NEIGHBOUR_COUNT; ++ n)
		{
			neighbour = current->neighbours[n];

			if (neighbour == nullptr || neighbour->blocked || closedList[GetIndex(neighbour->x, neighbour->y)])
				continue;

			int index = openList.Find(neighbour);

			int newScore = current->gScore + BASE_COSTS[n] + current->cost + current->neighbours[n]->cost;

			//Index found
			if (index != -1)
			{
				if (newScore < neighbour->gScore)
				{
					neighbour->gScore = newScore;
					neighbour->previous = current;
					openList.Rebalance(neighbour);
				}
			}
			else
			{
				neighbour->gScore = newScore;
				neighbour->previous = current;
				openList.Push(neighbour);
			}
		}
	}
	return true;
}

int Pathfinder::GetIndex(int x, int y)
{
	return y * gridWidth + x;
}
