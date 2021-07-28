#pragma once
#include "PathfinderNode.h"
#include "Heap.h"
#include "SoundFieldNode.h"
#include "Vector2.h"

using namespace MathMan;

#define COST_DIAG 14
#define COST_SIDE 10
#define DIAG_VEC Vector2(1, 1).Normalised();

class Pathfinder
{

public:
	//---------------------------------------
	//	Used to mark diagonals and adjacents
	//	14 | 10 | 14
	// 	10 | -- | 10
	// 	14 | 10 | 14
	//``````````````````````````````````````
	static constexpr int BASE_COSTS[8] = { COST_DIAG, COST_SIDE, COST_DIAG, COST_SIDE, COST_SIDE, COST_DIAG, COST_SIDE, COST_DIAG };
	
	Pathfinder(int width, int height, float gridSize);
	~Pathfinder();

	int GetIndex(int x, int y);
	PathfinderNode* GetNode(int x, int y);
	PathfinderNode* GetNode(int index);
	
	PathfinderNode* GetNodeFromPos(Vector2 pos);

	Vector2 GetNodeCoordinatesFromPos(Vector2 pos);

	Vector2 GetPosFromNode(PathfinderNode* node);

	PathfinderNode* GetNeighbourFromIndex(int x, int y, int neighbour);

	bool CreateFlowField(SoundFieldNode*& outVecField, Vector2 targetPos);

	int GetWidth();
	int GetHeight();
	
protected:
	//Creates a flowfield that paths to the startNode
	bool Dijkstras(PathfinderNode* startNode);

	//An array of pointers to pathfindernodes
	PathfinderNode** grid = nullptr;
	//Size of each cell in pixels
	float gridSize = 32;
	//Dims of grid in cells
	int gridWidth = 0;
	int gridHeight = 0;
	
	Heap openList;
	char* closedList = nullptr;

	
};