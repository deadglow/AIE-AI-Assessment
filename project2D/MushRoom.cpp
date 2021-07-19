#include "MushRoom.h"
#include <iostream>
#include <fstream>

MushRoom::MushRoom(std::string path)
{
    LoadMapData(path);
}

MushRoom::~MushRoom()
{
    delete[] mapData;
}

int* MushRoom::GetMapData()
{
    return mapData;
}

void MushRoom::LoadMapData(std::string path)
{
	//-----------------------------------------------
	//			Map data layout
	//	:width height
	//	:i i i i i i i /n
	//	:i i i i i i i /n
	//	:i i i i i i i /n
	//	:i i i i i i i /n
	//_______________________________________________

	//Load the map
	std::ifstream mushroomFile(path.c_str());

	//Cache for reading from lines
	std::string line;
	std::string str = "";

	//First line is the dimensions of the map separated by a space

	std::getline(mushroomFile, line);
	
	//Iterator to loop through line chars
	auto iter = line.begin();
	//Iterate through line chars until a space is found
	while (*iter != ' ')
	{
		str.push_back(*iter);
		iter++;
	}

	//Found width, assign then reset string
	width = std::stoi(str);
	str = "";

	//Move iter forward to skip space
	iter++;

	//Height is the rest of the chars after space
	while (iter != line.end())
	{
		str.push_back(*iter);
		iter++;
	}

	//Found height
	height = std::stoi(str);
	str = "";

	//Create mapData on heap as a 1d array
	mapData = new int[width * height];

	//Indices for coordinates
	int y = 0;
	int x;

	//Start reading data line by line
	while (std::getline(mushroomFile, line))
	{
		//Reset x position since its a new line
		x = 0;


		//Iterate through every char in line
		auto iter = line.begin();
		while (iter != line.end())
		{
			//' ' is the separator between cells
			//Allows for numbers larger than one character
			//(which is probably never going to be used)
			if (*iter == ' ')
			{
				//Separator found, so add the data to mapData
				SetData(x, y, std::stoi(str));
				iter++;
				x++;
				//Data added, reset string
				str = "";
			}
			else
			{
				str.push_back(*iter);
				iter++;
			}
		}

		//Reached end of line, go to the next one
		y++;
	}
}

int MushRoom::GetWidth()
{
    return width;
}

int MushRoom::GetHeight()
{
    return height;
}

int MushRoom::GetData(int x, int y)
{
	//Access 1d array like a 2d array
	return mapData[y * width + x];
}

void MushRoom::SetData(int x, int y, int data)
{
	mapData[y * width + x] = data;
}

