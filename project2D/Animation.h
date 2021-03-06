#pragma once
#include <vector>
#include "Texture.h"

class Animation
{
public:
	Animation();
	~Animation();

	aie::Texture* GetFrame(int index);
	void AddFrame(aie::Texture* tex);

	int GetFrameCount();

	std::string GetName();
	void SetName(std::string newName);

protected:
	std::string name = "Animation";
	//Container for a set of frames, which are pointers to textures
	std::vector<aie::Texture*> frames;
};

