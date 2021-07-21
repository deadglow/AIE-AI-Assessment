#pragma once

#include <string>

//Represents data for room creation
class MushRoom
{
public:
	MushRoom(std::string path);
	~MushRoom();

	int* GetMapData();
	void LoadMapData(std::string path);

	int GetWidth();
	int GetHeight();

	int GetData(int x, int y);
	void SetData(int x, int y, int data);
protected:
	std::string mapName = "";
	//2D grid of ints represented as a 1d array
	int* mapData = nullptr;
	int width = 0;
	int height = 0;
};